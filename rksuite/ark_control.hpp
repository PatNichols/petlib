#pragma once
#include <cmath>

namespace petlib {
namespace runge_kutta {

struct ark_control
{
    double eps_abs;
    double eps_rel;
    double max_dt;
    double min_dt;
    double a_y;
    double a_dydt;

   explicit ark_control(
       const double& eps_abs_,
       const double& eps_rel_,
       const double& max_dt_,
       const double& min_dt_,
       const double& a_y_,
       const double& a_dydt_):
       eps_abs(eps_abs_),
       eps_rel(eps_rel_),
       max_dt(max_dt_),
       min_dt(min_dt_),
       a_y(a_y_),
       a_dydt(a_dydt_)
   {
   } 

   explicit ark_control(
       const double& eps_abs_,
       const double& eps_rel_,
       const double& max_dt_,
       const double& min_dt_):
       eps_abs(eps_abs_),
       eps_rel(eps_rel_),
       max_dt(max_dt_),
       min_dt(min_dt_),
       a_y(1.0),
       a_dydt(0.0)
   {
   } 

   explicit ark_control(
       const double& eps_abs_,
       const double& max_dt_,
       const double& min_dt_):
       eps_abs(eps_abs_),
       eps_rel(0.0),
       max_dt(max_dt_),
       min_dt(min_dt_),
       a_y(1.0),
       a_dydt(0.0)
   {
   } 

        
    bool check_error(double& dt,
        double err_,
        double err_y,
        double err_dydt,
        int q,
        int& ext_step)
    {
        ext_step = 0;
        double S = 0.9;
        double err_term = a_y * err_y + dt * a_dydt * err_dydt;
        double D = eps_abs + eps_rel * err_term;
        double EoverD = err_/D;
        // if E > D * 1.1
        // E exceeds D by more than 10%  
        if ( EoverD > 1.1) {
            // reduce timestep error > max allowed
            dt = S * dt / pow(EoverD,q);
            if ( dt < min_dt) {
                dt = min_dt;
                ext_step = 1;
            }
            return false;
        }else{
            // inflate timestep
            if ( EoverD < 0.5) {
                dt = S *  dt / pow(EoverD,(q+1));
                if ( dt > max_dt) {
                    dt = max_dt;
                    ext_step = 1;
                }
            }
            return true;
        }    
    }

    friend std::ostream& operator << ( std::ostream& os, const ark_control& c)
    {
        os << "arkcontrol2 \n";
        os << " eps real   = " << c.eps_rel << " eps abs = " << c.eps_abs << "\n";
        os << " max dt     = " << c.max_dt << " min dt = " << c.min_dt << "\n";
        os << " y scale    = " << c.a_y << "\n";
        os << " dydt scale = " << c.a_dydt << "\n";
        return os;
    }
};

}
}