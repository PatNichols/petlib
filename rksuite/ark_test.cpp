#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include "rk_stepper.hpp"
#include "ark_stepper.hpp"
#include "ark_cofs.hpp"
#include "rk_cofs.hpp"
#include "ark_control.hpp"
#include "rk_sys.hpp"

bool is_done(double t,double tf,double min_dt)
{
    double d = fabs(t - tf);
    if ( d < min_dt) return true;
    return false;
}

void test1(const double& dt)
{
    using namespace petlib::runge_kutta;
    int nprint = 10;
    int iprint = 0;
    double t0 = 1.0;
    std::vector<double> y0(1);
    y0[0] = 1.0;
    SimpleTanDerivs sys;    
    double eps_abs = 1.e-4;
    double eps_rel = 1.e-6;
    double max_dt = dt+dt;
    double min_dt = dt * 0.1;
    ark_control con(eps_abs,eps_rel,max_dt,min_dt);
    rk_stepper< SimpleTanDerivs, RK4> rk4(y0,t0,dt,sys);
    adaptive_rk_stepper< SimpleTanDerivs, RK4Fehlberg, ark_control > ark4(y0,t0,dt,con,sys);
    adaptive_rk_stepper< SimpleTanDerivs, HeunEuler, ark_control > ark2(y0,t0,dt,con,sys);

    std::cout << con;

    double tf = 1.1000;
    size_t nsteps = static_cast<size_t>( std::round((tf-t0)/dt) );
    
    std::cout << "rk4\n";
    std::cout << rk4;
    for (size_t j=0;j<nsteps;++j) {
        rk4.step();
        std::cout << rk4;
    }
    std::cout << rk4 << "\n";
    std::cout << "-------------------\n";    
    std::cout << "ark4\n";
    std::cout << ark4;
    iprint = 1;
    while ( !is_done(ark4.current_time(),tf,min_dt) ) {
        ark4.step();
        if ( (iprint % nprint ) == 0 )
            std::cout << ark4;
        ++iprint;
    }
    std::cout << ark4 << "\n";
    std::cout << "-------------------\n";    
    std::cout << "ark2\n";
    std::cout << ark2;
    iprint = 1;
    while ( !is_done(ark2.current_time(),tf,min_dt) ) {
        ark2.step();
        if ( (iprint % nprint ) == 0 )
            std::cout << ark2;
        ++iprint;
    }
    std::cout << ark2 << "\n";
    std::cout << "-------------------\n";    
    std::cout << " done test 1\n";
    std::cout << "-------------------\n";    
}

void test2(const double& dt)
{
    using namespace petlib::runge_kutta;
    int nprint = 10;
    int iprint = 0;
    double t0 = 1.0;
    std::vector<double> y0(1);
    y0[0] = 1.0;
    SimpleTanDerivs sys;    
    double eps_abs = 1.e-4;
    double eps_rel = 1.e-6;
    double max_dt = dt * 2.0;
    double min_dt = dt * 0.1;
    ark_control con(eps_abs,eps_rel,max_dt,min_dt,0.0,0.0);
    rk_stepper< SimpleTanDerivs, RK4> rk4(y0,t0,dt,sys);
    adaptive_rk_stepper< SimpleTanDerivs, RK4Fehlberg, ark_control > ark4(y0,t0,dt,con,sys);
    adaptive_rk_stepper< SimpleTanDerivs, HeunEuler, ark_control > ark2(y0,t0,dt,con,sys);

    std::cout << con;

    double tf = 1.1000;
    size_t nsteps = static_cast<size_t>( std::round((tf-t0)/dt) );
    
    std::cout << "rk4\n";
    std::cout << rk4;
    for (size_t j=0;j<nsteps;++j) {
        rk4.step();
        std::cout << rk4;
    }
    std::cout << rk4 << "\n";
    std::cout << "-------------------\n";    
    std::cout << "ark4\n";
    std::cout << ark4;
    iprint = 1;
    while ( !is_done(ark4.current_time(),tf,min_dt) ) {
        ark4.step();
        if ( (iprint % nprint ) == 0 )
            std::cout << ark4;
        ++iprint;
    }
    std::cout << ark4 << "\n";
    std::cout << "-------------------\n";    
    std::cout << "ark2\n";
    std::cout << ark2;
    iprint = 1;
    while ( !is_done(ark2.current_time(),tf,min_dt) ) {
        ark2.step();
        if ( (iprint % nprint ) == 0 )
            std::cout << ark2;
        ++iprint;
    }
    std::cout << ark2 << "\n";
    std::cout << "-------------------\n";    
    std::cout << " done test 2\n";
    std::cout << "-------------------\n";    
}



void test3(const double& dt)
{
    using namespace petlib::runge_kutta;
    double t0 = 0.0;
    std::vector<double> y0(2);
    y0[0] = 0.5;
    y0[1] = 0.5;
    double eps_abs = 1.e-6;
    double max_dt = dt+dt;
    double min_dt = dt * 0.1;
    LoktaVolterra sys;    
    ark_control con(eps_abs,max_dt,min_dt);
    rk_stepper< LoktaVolterra, RK4> rk4(y0,t0,dt,sys);
    adaptive_rk_stepper< LoktaVolterra, RK4Fehlberg, ark_control > ark4(y0,t0,dt,con,sys);
    adaptive_rk_stepper< LoktaVolterra, HeunEuler, ark_control > ark2(y0,t0,dt,con,sys);

    std::cout << con;

    double tf = 3.0;
    size_t nsteps = static_cast<size_t>( std::round((tf-t0)/dt) );
    
    std::cout << "rk4\n";
    std::cout << rk4;
    for (size_t j=0;j<nsteps;++j) {
        rk4.step();
//        std::cout << rk4;
    }
    std::cout << rk4 << "\n";
    std::cout << "-------------------\n";    
    std::cout << "ark4\n";
    std::cout << ark4;
    int iprint = 1;
    int nprint = 10;
    while ( !is_done(ark4.current_time(),tf,min_dt) ) {
        ark4.step();
//        std::cout << ark4;
//        if ( (iprint % nprint ) == 0 )
//            std::cout << ark4;
//        ++iprint;
    }
    std::cout << ark4 << "\n";
    std::cout << "-------------------\n";    
    std::cout << "ark2\n";
    std::cout << ark2;
    iprint = 1;
    while ( !is_done(ark2.current_time(),tf,min_dt) ) {
        ark2.step();
//        if ( (iprint % nprint ) == 0 )
//            std::cout << ark2;
//        ++iprint;
    }
    std::cout << ark2 << "\n";
    std::cout << "-------------------\n";    
    std::cout << " done test LoktaVolterra\n";
    std::cout << "-------------------\n";    

}


void test4(const double& dt)
{
    using namespace petlib::runge_kutta;
    double t0 = 0.0;
    std::vector<double> y0(3);
    y0[0] = 2.0;
    y0[1] = 1.0;
    y0[2] = 1.0;
    double eps_abs = 1.e-6;
    double eps_rel = 1.e-4;
    double max_dt = dt+dt;
    double min_dt = dt * 0.01;
    Lorenz sys;    
    ark_control con(eps_abs,max_dt,min_dt);
    rk_stepper< Lorenz, RK4> rk4(y0,t0,dt,sys);
    adaptive_rk_stepper< Lorenz, RK4Fehlberg, ark_control > ark4(y0,t0,dt,con,sys);
    adaptive_rk_stepper< Lorenz, HeunEuler, ark_control > ark2(y0,t0,dt,con,sys);

    std::cout << con;

    double tf = 30.0;
    size_t nsteps = static_cast<size_t>( std::round((tf-t0)/dt) );
    
    std::cout << "rk4\n";
    std::cout << rk4;
    for (size_t j=0;j<nsteps;++j) {
        rk4.step();
//        std::cout << rk4;
    }
    std::cout << rk4 << "\n";
    std::cout << "-------------------\n";    
    std::cout << "ark4\n";
    std::cout << ark4;
    int iprint = 1;
    int nprint = 10;
    while ( !is_done(ark4.current_time(),tf,min_dt) ) {
        ark4.step();
//        std::cout << ark4;
//        if ( (iprint % nprint ) == 0 )
//            std::cout << ark4;
//        ++iprint;
    }
    std::cout << ark4 << "\n";
    std::cout << "-------------------\n";    
    std::cout << "ark2\n";
    std::cout << ark2;
    iprint = 1;
    while ( !is_done(ark2.current_time(),tf,min_dt) ) {
        ark2.step();
//        if ( (iprint % nprint ) == 0 )
//            std::cout << ark2;
//        ++iprint;
    }
    std::cout << ark2 << "\n";
    std::cout << "-------------------\n";    
    std::cout << " done test Lorenz\n";
    std::cout << "-------------------\n";    

}


int main()
{
    test1(0.025);
    test1(0.0125);
    test2(0.0125);
    test3(0.005);
    test4(0.02);
}
