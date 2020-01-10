#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace putils {

struct FileCache {
  FILE **fp;
  char *buffer;
  std::size_t buffer_size;
  long curr_file;
  std::string prefix;
  bool reading;
  bool has_ferror;
  bool end_of_cache;
  bool perserve;
  constexpr long MAX_CACHE_FILE_SIZE = 1048576 * 1024 * 32;
  constexpr long MAX_BUFFER_SIZE = 1048576 * 1024;

  FileCache() = delete;

  FileCache(const std::filesystem::path dir_path,
            const std::string &file_prefix_,
            std::size_t bufferSize = MAX_BUFFER_SIZE)
      : fp(nullptr),
        buffer(nullptr),
        buffer_size(bufferSize),
        curr_file(-1),
        prefix(dir_path + file_prefix_),
        reading(false),
        has_ferror(false),
        end_of_cache(false),
        perserve(false) {
    std::filesystem::create_directory(dir_path);
    fp = (FILE **)malloc(sizeof(FILE *));
    buffer = new char[buffer_size];
    curr_file = -1;
    open_memstream();
  }

  FileCache(const std::filesystem::path dir)
      : fp(nullptr),
        buffer(nullptr),
        buffer_size(0),
        curr_file(-1),
        prefix(dir),
        reading(true),
        has_ferror(false),
        end_of_cache(false),
        perserve(false) {
    std::filesystem::dir_(dir);
    std::filesystem::directory_iterator iter(dir);
    std::filesystem::director_entry f0(*iter);
    if (!f0.is_regular_file()) {
      std::cerr << "Weird file in cache directory not a regular file\n";
      exit(EXIT_FAILURE);
    }
    std::filesystem::path p0(f0.path());
    prefix = p0.filename();
    buffer_size = p0.file_size();
    for (auto &p : iter) {
      if (!p.is_regular_file()) {
        std::cerr << "Weird file in cache directory not a regular file\n";
        std::cerr << p.filename() << "\n";
        exit(EXIT_FAILURE);
      }
      size_t sz = p.file_size();
      fsizes.push_back(sz);
    }
    fp = (FILE **)malloc(sizeof(FILE *));
    buffer = new char[buffer_size];
    curr_file = -1;
    open_memstream();
  }

  ~FileCache() {
    close();
    if (!perserve)
      RemoveFiles();
    else {
      std::string name = generate_filename(0);
      *fp = Fopen(name, "w");
      Fwrite(buffer, 1, fsize[0], *fp);
      fclose(*fp);
    }
    delete[] buffer;
    free(fp);
  }

  void SetPerserve() { perserve = true; }

  void Open(std::string &openstr = std::string("r+")) {
    if (*fp) fclose(*fp);
    curr_file = 0;
    *fp = Fmemopen(reinterpret_cast<void *>(buffer), bsize, openstr.c_str());
  }

  void Close() {
    if (!*fp) return;
    if (reading) {
      close_current_input_file();
      curr_file = -1;
    } else {
      close_current_output_file();
      curr_file = -1;
    }
  }

  template <typename T>
  constexpr size_t getData(T *data_ptr, size_t n) {
    long pos = ftellg(*fp);
    long f_end = fsizes[curr_file];
    size_t dsize = sizeof(T) * n;
    size_t left = f_end - pos;
    if (dsize <= left) {
      size_t nrd = Fread(data_ptr, sizeof(T), n, *fp);
      if (nrd != n) has_ferror = true;
      return nrd;
    } else {
      size_t n_now = left / sizeof(T);
      if (n_now) {
        size_t n_rd = Fread(data_ptr, sizeof(T), n_now, *fp);
        if (n_rd != n_now) {
          has_ferror = true;
          return n_rd;
        }
      }
      if ((curr_file + 1L) == fsizes.size()) {
        end_of_cache = true;
        return n_now;
      }
      close_current_input_file();
      open_new_file_for_reading();
      size_t nex = getData<T>(data_ptr + n_now, (n - n_now));
      return (n_now + nex);
    }
  }

  template <typename T>
  constexpr size_t putData(T *data_ptr, size_t n) {
    if (reading) {
      std::cerr << "need to close cache and clear to put data!\n";
      exit(EXIT_FAILURE);
    }
    long pos = ftellg(*fp);
    long f_end = MAX_CACHE_FILE_SIZE;
    size_t dsize = sizeof(T) * n;
    size_t left = f_end - pos;
    if (dsize <= left) {
      size_t nwr = Fwrite(data_ptr, sizeof(T), n, *fp);
      if (nwr != n) has_ferror = true;
      return nwr;
    } else {
      size_t n_now = left / sizeof(T);
      if (n_now) {
        size_t nwr = Fwrite(data_ptr, sizeof(T), n_now, *fp);
        if (nwr != n_now) {
          has_ferror = true;
          return nwr;
        }
      }
      close_current_output_file();
      open_new_file_for_writing();
      size_t nex = putData<T>(data_ptr + n_now, (n - n_now));
      return (n_now + nex);
    }
  }

  constexpr void ClearError() {
    has_ferror = false;
    end_of_cache = false;
  }

  constexpr bool EndOfCache() const noexcept { return end_of_cache; }

  constexpr bool Error() const noexcept { return has_ferror; }

  constexpr bool Reading() const noexcept { return reading; }
  constexpr bool Writing() const noexcept { return !reading; }
  constexpr size_t NumberOfFiles() const noexcept { return fsizes.size(); }
  constexpr size_t TotalSize() const noexcept {
    size_t sum = 0;
    for (auto i = 0; i < fsizes.size(); ++i) sum += fsizes[i];
    return sum;
  }
  constexpr bool Perserve() const noexcept { return perserve; }
  constexpr bool InMemory() const noexcept { return fsizes.size() == 1; }
  constexpr void RemoveFiles() const noexcept {
    close();
    for (auto i = 0; i < fsizes.size(); ++i) {
      std::filesystem::path p = generate_filename(i);
      std::filesystem::remove(p, ec);
      if (ec) {
        std::cerr << "failed to remove file " << p << " error code = " << ec
                  << "\n";
      }
    }
  }
  constexpr void Rewind() noexcept {
    clearerr();
    if (mode == WRITE) {
      close_current_output_file();
    }
    if (mode == READ) {
      close_current_input_file();
    }
    curr_file = -1;
    open_new_file_for_reading();
  }

  constexpr void Clear() noexcept {
    curr_file = -1;
    fsizes.clear();
    open_new_file_for_writing();
  }

  constexpr std::string Prefix() const noexcept { return prefix; }

 private:
  std::string generate_filename(long no) {
    std::string fname = prefix;
    fname += std::to_string(no);
    fname += ".DAT";
    return fname;
  }

  void open_memstream_buffer() {
    if (reading) {
      *fp = Fmemopen(reinterpret_cast<void *>(buffer), bsize, "r");
    } else {
      *fp = Fmemopen(reinterpret_cast<void *>(buffer), bsize, "w");
    }
  }

  void open_new_file_for_writing() {
    reading = false;
    if (*fp) {
      fclose(*fp);
    }
    if (curr_file == -1) {
      open_memstream_buffer();
      curr_file = 0;
      fsizes.size() = 0;
      fsizes.clear();
    } else {
      curr_file += 1;
      std::string new_name = generate_filename(curr_file);
      *fp = Fopen(new_name.c_str(), "w");
      if (fsizes.size() > curr_file)
        fsizes[curr_file] = 0;
      else
        fsizes.push_back(0L);
    }
  }
  void open_new_file_for_reading() {
    reading = true;
    if (*fp) {
      fclose(*fp);
    }
    if (curr_file == -1) {
            open_memstream_buffer(std:string("r");    
            curr_file = 0;
    } else {
      curr_file += 1;
      std::string new_name = generate_filename(curr_file);
      *fp = Fopen(new_name.c_str(), "r");
    }
  }
  void close_current_output_file() {
    long fpos = ftellg(*fp);
    fclose(*fp);
    fsizes.push_back(fpos);
  }
  void close_current_input_file() { fclose(*fp); }

  constexpr FILE *Fopen(const char *restrict name,
                        const char *restrict mode) noexcept {
    FILE *fp = fopen(name, mode);
    if (fp == nullptr) {
      int err = errno;
      std::cerr << "Unable to open the file " << name << " in mode " << mode
                << "\n";
      std::cerr << "Error " << strerror(errno);
      exit(EXIT_FAILURE);
    }
    return fp;
  }
  constexpr FILE *Fmemopen(void *buff, size_t size, const char *mode) noexcept {
    FILE *fp = fmemopen(buff, size, mode);
    if (fp == nullptr) {
      int err = errno;
      std::cerr << "Unable to open the mem file of size " << size << " in mode "
                << mode << "\n";
      std::cerr << "Error " << strerror(errno);
      exit(EXIT_FAILURE);
    }
    return fp;
  }

  constexpr size_t Fread(void *restrict buffer, size_t size_, size_t count,
                         FILE *restrict fptr) {
    size_t nr = fread(buffer, size_, count, fptr);
    if (nr != count) {
      if (feof(fptr)) {
        std::cerr << "end of file reached in Fread\n";
        std::cerr << "warning check the return code\n";
        has_ferror = true;
      } else if (ferror(fptr)) {
        std::cerr << "error in reading file " << strerror(errno) << "\n";
        has_ferror = true;
      }
    }
    return nr;
  }
  constexpr size_t Fwrite(void *restrict buffer, size_t size_, size_t count,
                          FILE *restrict fptr) {
    size_t nw = fwrite(buffer, size_, count, fptr);
    if (nw != count) {
      if (feof(fptr)) {
        std::cerr << "end of file reached in Fwrite\n";
        std::cerr << "warning check the return code\n";
        has_ferror = true;
      } else if (ferror(fptr)) {
        std::cerr << "error in writing file " << strerror(errno) << "\n";
        has_ferror = true;
      }
    }
    return nw;
  }
};

}  // namespace putils