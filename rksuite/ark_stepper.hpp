#pragma once
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>
#include "rk_stepper.hpp"
namespace petlib {
namespace runge_kutta {

template < class rk_control_type >
class rk_control
{
    rk_control_type c;
public:    
    rk_control(const rk_control_type& c0):
        c(c0)
    {}

    bool check_error(double& dt,
        double err_,
        double err_y,
        double err_dydt,
        int q,
        int& extreme_step)
    {
        return c.check_error(dt,err_,err_y,err_dydt,q,extreme_step);
    }    
};

template < class cofs_t >
class ark_cofs
{
public:
    static int q() noexcept { return cofs_t::q();}
    static int num_steps() noexcept { return cofs_t::num_steps();}
    static void initialize(std::vector<std::vector<double>>& a,
                    std::vector<double>& c,
                    std::vector<double>& b,
                    std::vector<double>& berr)
    {
        return cofs_t::initialize(a,c,b,berr);
    }
};

template < class system_type, class acofs_type, class control_type >
class adaptive_rk_stepper
{
    rk_control<control_type> con;
    rk_system<system_type> sys;
    std::vector< std::vector<double> > m_derivs;
    std::vector<double> m_y;
    std::vector<double> m_ytmp;
    std::vector< std::vector<double> > m_a;
    std::vector<double> m_b;
    std::vector<double> m_c;
    std::vector<double> m_b_err;
    double m_t;
    double m_dt;
    size_t n_steps;
    size_t n_vars;
    int q;
public:
    adaptive_rk_stepper() = delete;
    adaptive_rk_stepper(
        std::vector<double>& y_init, const double& t_init,
        const double& delta_t, 
        const control_type& con0,
        const system_type& sys0 = system_type()):
        con(con0),
        sys(sys0),
        m_derivs(),
        m_y(y_init),
        m_ytmp(sys.num_vars()),
        m_t(t_init),
        m_dt(delta_t),
        n_steps( ark_cofs<acofs_type>::num_steps() ),
        n_vars(sys.num_vars()),
        q(ark_cofs<acofs_type>::q())
    {
        ark_cofs<acofs_type>::initialize(m_a,m_c,m_b,m_b_err);
        m_derivs = std::move(std::vector< std::vector<double> >(n_steps));
        for (int i=0;i<n_steps;++i) m_derivs[i] = std::vector<double>(n_vars);
        
    }

    const std::vector<double>& current_y() const noexcept { return m_y;}
    const double& current_time() const noexcept { return m_t;}
    const double& time_step() const noexcept { return m_dt;}

    void step() noexcept
    {
        int n_tries = 0;
        int max_tries = 1000000;
        bool done = false;
        int is_extreme = 0;
        for (int n = 0; n < max_tries; ++n) {
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
            double max_err = 0.;
            double max_y_err = 0.;
            double max_dydt_err = 0.;
            for (size_t j=0; j<n_vars; ++j) {
                double err_sum = 0.0;
                double sum = 0.0;
                for (size_t k=0; k<n_steps; ++k) {
                    sum += m_b[k] * m_derivs[k][j];
                    err_sum += m_b_err[k] * m_derivs[k][j];
                }
                m_ytmp[j] = m_y[j] + sum * m_dt;
                double err_ = fabs(err_sum);
                if ( err_ > max_err ) {
                    max_err = err_;
                    max_y_err = fabs(m_ytmp[j]);
                    max_dydt_err = fabs(sum);
                }
            }
            if ( is_extreme == 1) { 
                done=1; 
                break; 
            }
            done = con.check_error(m_dt,max_err,max_y_err,max_dydt_err,q,is_extreme);
            if ( done ) break;
        }
        if (!done) {
            std::cerr << "warning step not completed in " << max_tries << " tries\n";
        }
        for (size_t j=0;j<n_vars;++j) m_y[j] = m_ytmp[j];
        m_t += m_dt;
    }

    std::ostream& print(std::ostream& os) const
    {
        os << std::setw(20) << std::setprecision(8) << std::fixed;
        os << "t = " << m_t << " dt = " << m_dt << " y = ";
        for (size_t j=0; j<n_vars; ++j) {
            os << " " << m_y[j];
        }
        os << "\n";
        return os;
    }
    friend std::ostream& operator << ( std::ostream& os, const adaptive_rk_stepper& s)
    {
        return s.print(os);
    }
};

}
}
