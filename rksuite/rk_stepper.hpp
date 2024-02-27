#pragma once
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <span>

namespace petlib 
{
namespace runge_kutta
{

/////////////////////////////////////////////////////
//  General wrapper around a system type
//  using the BN trick.
//  Any implemented system type should define
//    the member functions num_vars and calculate_derivs
/////////////////////////////////////////////////////
template < class system_t >
class rk_system
{
public:
    rk_system():sys() {}
    rk_system(const system_t& sys0):sys(sys0) {}
    constexpr int num_vars() const noexcept
    {
        return sys.num_vars();
    }
    constexpr void calculate_derivs(const std::vector<double>& y,const double& t,
            std::vector<double>& derivs) const noexcept
    {
        return sys.calculate_derivs(y,t,derivs);
    }
private:
    system_t sys;
};

/////////////////////////////////////////////////////
//  General wrapper around a runge-kutta coefficients type
//  using the BN trick.
//  Any implemented system type should define
//    the member functions a,b,c,num_stages
/////////////////////////////////////////////////////
template < class cofs_t >
class rk_cofs
{
public:
    static void initialize(
        std::vector< std::vector<double> >& a,
        std::vector<double>& b,
        std::vector<double>& c) noexcept
    {
        cofs_t::initialize(a,b,c);    
    }
    static int num_steps() noexcept { return cofs_t::num_steps();}
};

template < class system_t, class cofs_t >
class rk_stepper
{
    rk_system<system_t> sys;
    std::vector< std::vector<double> > m_derivs;
    std::vector<double> m_y;
    std::vector<double> m_ytmp;
    std::vector< std::vector<double> > m_a;
    std::vector<double> m_b;
    std::vector<double> m_c;
    double m_t;
    double m_dt;
    size_t n_steps;
    size_t n_vars;
public:
    rk_stepper() = delete;
    rk_stepper(
        std::vector<double>& y_init, const double& t_init,
        const double& delta_t, const system_t& sys0 = system_t()):
        sys(sys0),
        m_derivs(),
        m_y(y_init),
        m_ytmp(sys0.num_vars()),
        m_t(t_init),
        m_dt(delta_t),
        n_steps(rk_cofs<cofs_t>::num_steps()),
        n_vars(sys0.num_vars())
    {
        rk_cofs<cofs_t>::initialize(m_a,m_b,m_c);
        m_derivs = std::move(std::vector< std::vector<double> >(n_steps));
        for (int i=0;i<n_steps;++i) m_derivs[i] = std::vector<double>(n_vars);
        
    }

    const std::vector<double>& current_y() const noexcept { return m_y;}
    const double& current_time() const noexcept { return m_t;}
    const double& time_step() const noexcept { return m_dt;}

    void step() noexcept
    {
        sys.calculate_derivs(m_y,m_t,m_derivs[0]);
        for (size_t i=1; i<n_steps; ++i) {
            for (size_t j=0; j<n_vars; ++j) {
                double sum = 0.0;
                for (size_t k=0; k<i; ++k) {
                    sum += m_derivs[k][j] * m_a[i-1][k];
                }
                m_ytmp[j] = m_y[j] + sum * m_dt ;
            }
            double ts = m_t + m_dt * m_c[i-1];
            sys.calculate_derivs(m_ytmp,ts,m_derivs[i]);
        }
        for (size_t j=0; j<n_vars; ++j) {
            double sum = 0.0;
            for (size_t k=0; k<n_steps; ++k) {
                sum += m_b[k] * m_derivs[k][j];
            }
            m_y[j] += sum * m_dt;
        }
        m_t += m_dt;
    }

    std::ostream& print(std::ostream& os)
    {
        os << std::setw(20) << std::setprecision(8) << std::fixed;
        os << "t = " << m_t;
        for (size_t j=0; j<n_vars; ++j) {
            os << " " << m_y[j];
        }
#ifdef PRINT_DEROVS_
        os << " derivs   ";
        for (size_t i=0;i<n_steps;++i) {
        for (size_t j=0;j<n_vars;++j) {
            os << " " << m_derivs[i][j];
        }
        }
#endif        
        os << "\n";
        return os;
    }
    friend std::ostream& operator << ( std::ostream& os, rk_stepper& s)
    {
        return s.print(os);
    }
};
}
}