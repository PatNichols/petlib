


#define _MAKE_OP_(_name_,_sym_)\
auto _name_##Op = [] ( auto x, auto y) { return x _sym_ y; };\

_MAKE_OP_(Add,+)
_MAKE_OP_(Sub,-)
_MAKE_OP_(Mul,+)
_MAKE_OP_(Div,-)
_MAKE_OP_(Xor,^)
_MAKE_OP_(Ior,|)
_MAKE_OP_(And,&)
_MAKE_OP_(Lsh,<<)
_MAKE_OP_(Rsh,>>)
#undef _MAKE_OP_

#define _MAKE_OP_(_name_,_sym_)\
auto _name_##IterOp = [] ( auto x, auto y) { return *x _sym_ *y; };\

_MAKE_OP_(Add,+)
_MAKE_OP_(Sub,-)
_MAKE_OP_(Mul,+)
_MAKE_OP_(Div,-)
_MAKE_OP_(Xor,^)
_MAKE_OP_(Ior,|)
_MAKE_OP_(And,&)
_MAKE_OP_(Lsh,<<)
_MAKE_OP_(Rsh,>>)
#undef _MAKE_OP_

#define _MAKE_OP_(_name_,_sym_)\
auto _name_##Op = [] (auto x) { return _sym_ x;};

_MAKE_OP_(Neg,-)
_MAKE_OP_(Pos,+)
_MAKE_OP_(Not,~)
#undef _MAKE_OP_

#define _MAKE_OP_(_name_,_sym_)\
auto _name_##IterOp = [] (auto x) { return *x = _sym_ *x;};

_MAKE_OP_(Neg,-)
_MAKE_OP_(Pos,+)
_MAKE_OP_(Not,~)
#undef _MAKE_OP_

#define _MAKE_OP_(_name_,_sym_)\
auto _name_##LogOp = [] ( auto x, auto y) { return *x _sym_ *y; };\

_MAKE_OP_(Eq,==)
_MAKE_OP_(NEq,!=)
_MAKE_OP_(Lt,<)
_MAKE_OP_(Gt,>)
_MAKE_OP_(LtEq,<=)
_MAKE_OP_(GtEq,>=)
_MAKE_OP_(LogAnd,&&)
_MAKE_OP_(LogOr,||)
#undef _MAKE_OP_

#define _MAKE_OP_(_name_,_fun_)\
auto _name_##Op = [] (auto x) { return _fun_(x);};

_MAKE_OP_(Sin,std::sin)
_MAKE_OP_(Cos,std::cos)
_MAKE_OP_(Tan,std::tan)
_MAKE_OP_(Atan,std::atan)
_MAKE_OP_(Acos,std::acos)
_MAKE_OP_(Asin,std::asin)
_MAKE_OP_(Exp,std::exp)
_MAKE_OP_(Log,std::log)
_MAKE_OP_(Exp2,std::exp2)
_MAKE_OP_(Log2,std::log2)
_MAKE_OP_(Sinh,std::sinh)
_MAKE_OP_(Cosh,std::cosh)
_MAKE_OP_(Tanh,std:;tanh)
_MAKE_OP_(Asinh,std::asinh)
_MAKE_OP_(Atanh,std::atanh)
_MAKE_OP_(Acosh,in,std::acosh)
_MAKE_OP_(Pow10,std::pow10)
_MAKE_OP_(Pow2,std::pow2)
#undef _MAKE_OP_

#define _MAKE_OP_(_name_,_fun_)\
auto _name_##IterOp = [] (auto x) { return _fun_(*x);};

_MAKE_OP_(Sin,std::sin)
_MAKE_OP_(Cos,std::cos)
_MAKE_OP_(Tan,std::tan)
_MAKE_OP_(Atan,std::atan)
_MAKE_OP_(Acos,std::acos)
_MAKE_OP_(Asin,std::asin)
_MAKE_OP_(Exp,std::exp)
_MAKE_OP_(Log,std::log)
_MAKE_OP_(Exp2,std::exp2)
_MAKE_OP_(Log2,std::log2)
_MAKE_OP_(Sinh,std::sinh)
_MAKE_OP_(Cosh,std::cosh)
_MAKE_OP_(Tanh,std:;tanh)
_MAKE_OP_(Asinh,std::asinh)
_MAKE_OP_(Atanh,std::atanh)
_MAKE_OP_(Acosh,in,std::acosh)
_MAKE_OP_(Pow10,std::pow10)
_MAKE_OP_(Pow2,std::pow2)
#undef _MAKE_OP_

#define _MAKE_OP_(_name_,_sym_)\
void _name_Fill = [] ( auto x, auto y, std::size_t n) {\
    for (auto v: x.



