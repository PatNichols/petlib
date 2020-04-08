


namespace petlib {

enum class MultiArrayOrder: std::uint8_t {
    RowMajor=0,
    ColMajor=1,
    Fortran=2,
    IDE=3
};


template < enum petlib::MultiArrayOrder > 
class Indexer
public:
{   typedef std::size_t size_type;

    Indexer( const std::initializer_list<size_type>& ilist) noexcept {
        ndim = ilist.size();
        tsize = 1;
        for (auto i=0;i<ndim;++i) {
            dims[i] = ilist[i];
            tsize *= ilist[i];
        }
        SetStrides();
    }
    template < size_type nd >
    Indexer( const std::array<size_type, nd>& dims_in) noexcept {
        ndim = nd;
        tsize = 1;
        for (auto i=0;i<nd;++i) {
            dims[i] = dims_in[i];
            tsize *= dims_in[i];
        }    
        SetStrides();
    }
    Indexer( std::size_t ndims_in, const size_type * dims_in) {
        ndim = ndims_in;
        tsize = 1;
        for (auto i=0;i<nd;++i) {
            dims[i] = dims_in[i];
            tsize *= dims_in[i];
        }        
        SetStrides();
    }
    Indexer( const std::vector<size_type>& dims_in) {
        ndim = dims_in.size();
        tsize = 1;
        for (auto i=0;i<nd;++i) {
            dims[i] = dims_in[i];
            tsize *= dims_in[i];
        }        
        SetStrides();
    }
    
    constexpr size_type IndicesToFlatIndex( const size_type* ind ) const noexcept {
        std::size_t f= ind[0]*strs[0] + ind[1]*strs[1] + ind[2]*strs[2] + ind[3]*strs[3]  + ind[4] * strs[4] + ind[5] * sts[5];
        return f;
    }
    constexpr size_type IndicesToFlatIndex( const std::array<size_type,5>& ind) const noexcept {
        std::size_t f= ind[0]*strs[0] + ind[1]*strs[1] + ind[2]*strs[2] + ind[3]*strs[3]  + ind[4] * strs[4] + ind[5] * sts[5];
        return f;    
    }
    constexpr size_type IndicesToFlatIndex( const std::vector<size_type>& ind ) const noexcept {
        std::size_t f= ind[0]*strs[0] + ind[1]*strs[1] + ind[2]*strs[2] + ind[3]*strs[3]  + ind[4] * strs[4] + ind[5] * sts[5];
        return f;        
    }
    constexpr size_type IndicesToFlatIndex( const std::initializer_list<size_type>& ind) const noexcept {
        std::size_t f= ind[0]*strs[0] + ind[1]*strs[1] + ind[2]*strs[2] + ind[3]*strs[3]  + ind[4] * strs[4] + ind[5] * sts[5];
        return f;        
    }
    void FlatIndexToIndices( size_type flatIndex,size_type* ind ) const noexcept ;
    void FlatIndexToIndices( size_type flatIndex,std::array<size_type,5>& ind) const noexcept;  
    void FlatIndexToIndices( size_type flatIndex,std::vector<size_type>& ind ) const noexcept;
    
    size_type NumberOfDimensions() const noexcept { return ndim;}
    size_type MaxDimensions() const noexcept { return 5;}
    size_type Extent( size_type i ) const noexcept { return dims[i];}
    size_type TotalExtent() const noexcept { return tsize;}
    std::array<size_type,5> GetExtents() const noexcept {
        return to_array(dims);
    }
private:        
    void SetStrides() noexcept;    
    size_type ndim,tsize;
    std::array< size_t, 5 > dims;
    std::array< size_t, 5 > strs;
};


constexpr void Indexer< MultiArrayOrder::RowMajor>::SetStrides() noexcept {
    std::size_type st = 1;
    for (std::size_t i=0;i<ndim;++i) {
        std::size_t k=ndim-1-i;
        strs[k] = st;
        st *= dims[k];
    }
    tsize = st;
}

constexpr void Indexer< MultiArrayOrder::ColMajor>::SetStrides() noexcept {
    std::size_type st = 1;
    for (std::size_t k=0;k<ndim;++k) {
        strs[k] = st;
        st *= dims[k];
    }
    tsize = st;
}

constexpr void Indexer< MultiArrayOrder::Fortran >::SetStrides() noexcept {
    std::size_type st = 1;
    for (std::size_t k=0;k<ndim;++k) {
        strs[k] = st;
        st *= dims[k];
    }
    tsize = st;
}

constexpr void Indexer< MultiArrayOrder::IDE >::SetStrides() noexcept {
    std::size_type st = 1;
    for (std::size_t i=0;i<ndim;++i) {
        std::size_t k = ndim-1-i;
        strs[k] = st;
        st *= dims[k];
    }
    tsize = st;
}

constexpr void Indexer< MultiArrayOrder::RowMajor>::FlatIndexToIndices( std::size_t flat_index, std::size_t *ix) const noexcept {
    for (std::size_t i=0;i<ndim;++i) {
        std::size_t k=ndim-1-i;
        ix[k] = (flat_index % dims[k]);
        flat_index /= dims[k];
    }

}

constexpr void Indexer< MultiArrayOrder::ColMajor>::FlatIndexToIndices( std::size_t flat_index, std::size_t *ix) const noexcept {
    for (std::size_t k=0;k<ndim;++k) {
        ix[k] = flat_index % dims[k];
        flat_index /= dims[k];
    }
}

constexpr void Indexer< MultiArrayOrder::Fortran >::FlatIndexToIndices( std::size_t flat_index, std::size_t *ix) const noexcept {
    for (std::size_t k=0;k<ndim;++k) {
        ix[k] = flat_index % dims[k] + 1;
        flat_index /= dims[k];
    }
}

constexpr void Indexer< MultiArrayOrder::IDE >::FlatIndexToIndices( std::size_t flat_index, std::size_t *ix) const noexcept {
    for (std::size_t i=0;i<ndim;++i) {
        std::size_t k = ndim-1-i;
        ix[k] = flat_index % dims[k] + 1;
        flat_index /= dims[k];
    }
}

constexpr void Indexer< MultiArrayOrder::RowMajor>::FlatIndexToIndices( std::size_t flat_index, std::array<std::size_t,5>& ix) const noexcept {
    for (std::size_t i=0;i<ndim;++i) {
        std::size_t k=ndim-1-i;
        ix[k] = (flat_index % dims[k]);
        flat_index /= dims[k];
    }
}

constexpr void Indexer< MultiArrayOrder::ColMajor>::FlatIndexToIndices( std::size_t flat_index, std::array<std::size_t,5>& ix) const noexcept {
    for (std::size_t k=0;k<ndim;++k) {
        ix[k] = flat_index % dims[k];
        flat_index /= dims[k];
    }
}

constexpr void Indexer< MultiArrayOrder::Fortran >::FlatIndexToIndices( std::size_t flat_index, std::array<std::size_t,5>& ix) const noexcept {
    for (std::size_t k=0;k<ndim;++k) {
        ix[k] = flat_index % dims[k] + 1;
        flat_index /= dims[k];
    }
}

constexpr void Indexer< MultiArrayOrder::IDE >::FlatIndexToIndices( std::size_t flat_index, std::array<std::size_t,5>& ix) const noexcept {
    for (std::size_t i=0;i<ndim;++i) {
        std::size_t k = ndim-1-i;
        ix[k] = flat_index % dims[k] + 1;
        flat_index /= dims[k];
    }
}

constexpr void Indexer< MultiArrayOrder::RowMajor>::FlatIndexToIndices( std::size_t flat_index, std::vector<std::size_t>& ix) const noexcept {
    for (std::size_t i=0;i<ndim;++i) {
        std::size_t k=ndim-1-i;
        ix[k] = (flat_index % dims[k]);
        flat_index /= dims[k];
    }

}

constexpr void Indexer< MultiArrayOrder::ColMajor>::FlatIndexToIndices( std::size_t flat_index, std::vector<std::size_t>& ix) const noexcept {
    for (std::size_t k=0;k<ndim;++k) {
        ix[k] = flat_index % dims[k];
        flat_index /= dims[k];
    }
}

constexpr void Indexer< MultiArrayOrder::Fortran >::FlatIndexToIndices( std::size_t flat_index, std::vector<std::size_t>& ix) const noexcept {
    for (std::size_t k=0;k<ndim;++k) {
        ix[k] = flat_index % dims[k] + 1;
        flat_index /= dims[k];
    }
}

constexpr void Indexer< MultiArrayOrder::IDE >::FlatIndexToIndices( std::size_t flat_index, std::vector<std::size_t>& ix) const noexcept {
    for (std::size_t i=0;i<ndim;++i) {
        std::size_t k = ndim-1-i;
        ix[k] = flat_index % dims[k] + 1;
        flat_index /= dims[k];
    }
}

}
#endif

