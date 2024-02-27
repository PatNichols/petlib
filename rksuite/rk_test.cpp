
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include "rk_stepper.hpp"
#include "rk_cofs.hpp"
#include "rk_sys.hpp"

void test1(const double& dt)
{
    using namespace petlib::runge_kutta;
    double t0 = 1.0;
    std::vector<double> y0(1);
    y0[0] = 1.0;
    SimpleTanDerivs sys;    
    
    rk_stepper< SimpleTanDerivs, RK4mod > step4mod(y0,t0,dt,sys);
    rk_stepper< SimpleTanDerivs, RK4> step4(y0,t0,dt,sys);
    rk_stepper< SimpleTanDerivs, Ralston > step2(y0,t0,dt,sys);
  
    double tf = 1.1;
    size_t nsteps = static_cast<size_t>( std::round((tf-t0)/dt) );
    
    std::cout << "rk4 mod\n";
    std::cout << step4mod;
    for (size_t j=0;j<nsteps;++j) {
        step4mod.step();
        std::cout << step4mod;
    }
    std::cout << "-------------------\n";    
    std::cout << "rk4\n";
    std::cout << step4;
    for (size_t j=0;j<nsteps;++j) {
        step4.step();
        std::cout << step4;
    }
    std::cout << "-------------------\n";    
    std::cout << "Ralston\n";
    std::cout << step2;
    for (size_t j=0;j<nsteps;++j) {
        step2.step();
        std::cout << step2;
    }
    std::cout << "-------------------\n";    
    std::cout << "test 1 done\n\n";
}

void test2(const double& dt)
{
    using namespace petlib::runge_kutta;
    double t = 0.0;
    std::vector<double> y(2);
    y[0] = 0.9;
    y[1] = 0.9;
    rk_system<LoktaVolterra> sys;        
    rk_stepper< LoktaVolterra, RK4mod > step4mod(y,t,dt);
    rk_stepper< LoktaVolterra, RK4> step4(y,t,dt);
    rk_stepper< LoktaVolterra, Midpoint > step2(y,t,dt);
  
    double tf = 5.0;
    size_t nsteps = size_t(rint(tf-t)/dt); 
    
    std::cout << "rk4 mod\n";
    std::cout << step4mod;
    for (size_t j=0;j<nsteps;++j) {
        step4mod.step();
        std::cout << step4mod;
    }
    std::cout << "-------------------\n";    
    std::cout << "rk4\n";
    std::cout << step4;
    for (size_t j=0;j<nsteps;++j) {
        step4.step();
        std::cout << step4;
    }
    std::cout << "-------------------\n";    
    std::cout << "midpoint\n";
    std::cout << step2;
    for (size_t j=0;j<nsteps;++j) {
        step2.step();
        std::cout << step2;
    }
    std::cout << "-------------------\n";    
    std::cout << "LoktaVolterra done\n\n";
}

void test3(const double& dt)
{
    using namespace petlib::runge_kutta;
    double t0 = 0.0;
    std::vector<double> y0(3);
    y0[0] = 2.0;
    y0[1] = 1.0;
    y0[2] = 1.0;

    Lorenz sys;        
    rk_stepper< Lorenz, RK4mod > step4mod(y0,t0,dt,sys);
    rk_stepper< Lorenz, RK4> step4(y0,t0,dt,sys);
    rk_stepper< Lorenz, Midpoint > step2(y0,t0,dt,sys);
  
    double tf = 30.0;
    size_t nsteps = size_t(rint(tf-t0)/dt); 
    std::cout << "# of steps = " << nsteps;    
    std::cout << "rk4 mod\n";
    std::cout << step4mod;
    for (size_t j=0;j<nsteps;++j) {
        step4mod.step();
//        std::cout << step4mod;
    }
    std::cout << step4mod;
    std::cout << "-------------------\n";    
    std::cout << "rk4\n";
    std::cout << step4;
    for (size_t j=0;j<nsteps;++j) {
        step4.step();
    }
    std::cout << step4;
    std::cout << "-------------------\n";    
    std::cout << "midpoint\n";
    std::cout << step2;
    for (size_t j=0;j<nsteps;++j) {
        step2.step();
//        std::cout << step2;
    }
    std::cout << step2;
    std::cout << "-------------------\n";    
    std::cout << "Lorenz done\n\n";
    std::cout << "-------------------\n";    
}

int main()
{
    test1(0.025);
    test1(0.0125);
//    test1(0.006255);
    test2(0.1);
    test3(0.02);
}
