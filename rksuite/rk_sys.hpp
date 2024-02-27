#pragma once
#include <cstdlib>
#include <vector>
#include <cmath>

namespace petlib
{
namespace runge_kutta
{

struct SimpleTanDerivs
{
    constexpr void calculate_derivs(
        const std::vector<double>& y,const double& t,std::vector<double>& derivs) const noexcept
    {
        derivs[0] = tan(y[0]) + 1.0;
    }
    constexpr int num_vars() const noexcept {
        return 1;
    }
};

struct SimpleSinDerivs
{
    double alpha_;

    SimpleSinDerivs(const double& alpha = 1.0):
      alpha_(alpha) {}  
   
    constexpr void calculate_derivs(
        const std::vector<double>& y,const double& t,std::vector<double>& derivs) const noexcept
    {
        double siny = sin(alpha_* y[0]);
        derivs[0] = siny * siny * y[0];
    }

    constexpr int num_vars() const noexcept {
        return 1;
    }
};

struct Lorenz
{
    double rho, sigma, beta;

    Lorenz(const double &rho_, const double &sigma_, const double &beta_)
        : rho(rho_), sigma(sigma_), beta(beta_) {}

    Lorenz() : rho(28.0), sigma(10.0), beta(2.6666666666667) {}

    constexpr void calculate_derivs(
        const std::vector<double>& y,const double& t,std::vector<double>& derivs) const noexcept
    {
        derivs[0] = sigma * (y[1] - y[0]);
        derivs[1] = y[0] * (rho - y[2]) - y[1];
        derivs[2] = y[0] * y[1] - beta * y[2];
    }

    constexpr int num_vars() const noexcept {
        return 3;
    }
};

struct LoktaVolterra
{
    double a,b,c,d;

    LoktaVolterra():
        a(0.66666666666667),
        b(1.33333333333333),
        c(1.),d(1.)
    {}

    LoktaVolterra(const double& alpha,const double& beta,const double& delta,const double& gamma):
        a(alpha),b(beta),c(delta),d(gamma)
    {}

    constexpr void calculate_derivs(const std::vector<double>& y,const double& t,
                          std::vector<double>& der) const noexcept
    {
        der[0] = (a  - b * y[1]) * y[0];
        der[1] = (c * y[0]  - d) * y[1];
    }
  
    constexpr size_t num_vars() const noexcept {
        return 2;
    }
};

}
}
