


struct indexer3d {
   std::array<std::size_t,3> dims;
   std::array<std::size_t,2> strs;

   indexer(size_t n1,size_t n2,size_t n3) {
       dims[0]=n1;
       dims[1]=n2;
       dims[2]=n3;
       strs[1]=n3;
       strs[0]=strs[1]*n2;
   }
   std:size_t index(size_t i,size_t j,size_t k) {
       return (i*strs[0]+j*strs[1]+k);
   }

   std::stride(size_t i) { return strs[i];};
   std::extent(size_t i) { return dims[i];};
   std::size(size_t i) { return dims[0]*dims[1]*dims[2];};
};
   
struct indexer4d {
   std::array<std::size_t,4> dims;
   std::array<std::size_t,3> strs;

   indexer(size_t n1,size_t n2,size_t n3,size_t n4) {
       dims[0]=n1;
       dims[1]=n2;
       dims[2]=n3;
       dims[3]=n4;
       strs[2]=n4;
       strs[1]=strs[2]*n3;
       strs[0]=strs[1]*n2;
   }
   std:size_t index(size_t i,size_t j,size_t k,size_t l) {
       return (i*strs[0]+j*strs[1]+k*strs[2]+l);
   }

   std::stride(size_t i) { return strs[i];};
   std::extent(size_t i) { return dims[i];};
   std::size(size_t i) { return dims[0]*dims[1]*dims[2]*dims[3];};
};
   
struct colmajor_indexer3d {
   std::array<std::size_t,3> dims;
   std::array<std::size_t,2> strs;

   colmajor_indexer(size_t n1,size_t n2,size_t n3) {
       dims[0]=n1;
       dims[1]=n2;
       dims[2]=n3;
       strs[1]=n1*n2;
       strs[0]=n1;
   }
   std:size_t index(size_t i,size_t j,size_t k) {
       return (i + j * strs[0] + k*strs[1]);
   }

   std::stride(size_t i) { return strs[i];};
   std::extent(size_t i) { return dims[i];};
   std::size(size_t i) { return dims[0]*dims[1]*dims[2];};
};
   
struct colmajor_indexer4d {
   std::array<std::size_t,4> dims;
   std::array<std::size_t,3> strs;

   colmajor_indexer(size_t n1,size_t n2,size_t n3,size_t n4) {
       dims[0]=n1;
       dims[1]=n2;
       dims[2]=n3;
       dims[3]=n4;
       strs[0]=n1;
       strs[1]=n1*n2;
       strs[3]=n1*n2*n3;
   }
   std:size_t index(size_t i,size_t j,size_t k,size_t l) {
       return (i + j *strs[0] + k *strs[1] + l*strs[2]);
   }
   
   std::array<std::size_t,4> make_index(size_t flat_index) {
       std::array
       std::size_t l = flat_index/strs[2];
       flat_index -= l *strs[3];
       std::size_t k = flat_index/strs[1];
       flat_index -= k *strs[2];
       std::size_t j = flat_index/strs[0];
       flat_index -= j *strs[0];
       std::size_t i = 
   };
   


   std::stride(size_t i) { return strs[i];};
   std::extent(size_t i) { return dims[i];};
   std::size(size_t i) { return dims[0]*dims[1]*dims[2]*dims[3];};
};
   
  
   