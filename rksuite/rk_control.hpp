#pragam once
#include <vector>
#include <cmath>

namespace petlib {
namespace runge_kutta {

class ark_control
{
    double eps_rel;
    double eps_abs;
    double ay_scale;
    double ady_scale;
    double scale;
    double max_dt;
    double min_dt;
  
    ark_control(
        const double& eps_abs_,
        const double& eps_rel_,
        const double& ay_scale_,
        const double& ady_scale_,
        const double& max_dt_,
        const double& min_dt_):
        eps_rel(eps_rel_),
        eps_abs(eps_abs_),
        ay_scale(ay_scale_),
        ady_scale(ady_scale_),
        max_dt(max_dt_),
        min_dt(min_dt_)
    {}

    ark_control(
        const double& eps_abs_,
        const double& eps_rel_,
        const double& max_dt_,
        const double& min_dt_):
        eps_rel(eps_rel_),
        eps_abs(eps_abs_),
        ay_scale(1.0),
        ady_scale(0.0),
        max_dt(max_dt_),
        min_dt(min_dt_)
    {}



    bool check_error(double& dt,
        double err_,
        double err_y,
        double err_dydt,
        int q,
        int& extreme_step)
    {
        extreme_step = 0;
        double D = eps_abs + eps_rel * ( err_y + err_dydt) * dt;
        double EoverD = err_/D;
        if (EoverD > 1.1) {
            // reduce step size
            dt = dt * S / pow(EoverD,q);
            if ( dt < min_dt) {
                dt = min_dt;
                extreme_step = 1;
            }
        } else {
            if ( EoverD < 0.5) {
                dt = dt * S / pow(EoverD,(q+1));
                if ( dt > max_dt ) {
                    dt = max_dt;
                    extreme_step = 1;
                }
            }
        }
    }    
};

}
}