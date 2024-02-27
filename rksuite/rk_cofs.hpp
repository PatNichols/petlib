#pragma once
#include <cstdlib>
#include <vector>

namespace petlib 
{
namespace runge_kutta
{

struct RK4mod
{
  static int num_steps() noexcept  { return 4; }

  static void initialize(std::vector<std::vector<double>>& m_a,
        std::vector<double>& m_b,std::vector<double>& m_c) noexcept
  {
    m_a = std::vector<std::vector<double>>(3);
    for (int i=0;i<3;++i) m_a[i] = std::vector<double>(i+1);

    m_b = std::move(std::vector<double>(4));

    m_c = std::move(std::vector<double>(3));

    m_b[0] = 1. / 8.;
    m_b[1] = 3. / 8.;
    m_b[2] = 3. / 8.;
    m_b[3] = 1. / 8.;

    m_a[0][0] = 1. / 3.;

    m_a[1][0] = -1. / 3.;
    m_a[1][1] = 1.0;

    m_a[2][0] = 1.0;
    m_a[2][1] = -1.0;
    m_a[2][2] = 1.0;

    m_c[0] = 1. / 3.;
    m_c[1] = 2. / 3.;
    m_c[2] = 1.0;

  }
};

struct RK4
{
  static int num_steps() noexcept  { return 4; }

  static void initialize(std::vector<std::vector<double>>& m_a,
        std::vector<double>& m_b,std::vector<double>& m_c) noexcept
  {
    m_a = std::vector<std::vector<double>>(3);
    for (int i=0;i<3;++i) m_a[i] = std::vector<double>(i+1);
    m_b = std::vector<double>(4);
    m_c = std::vector<double>(3);

    m_b[0] = 1. / 6.;
    m_b[1] = 1. / 3.;
    m_b[2] = 1. / 3.;
    m_b[3] = 1. / 6.;

    m_a[0][0] = 0.5;

    m_a[1][0] = 0.0;
    m_a[1][1] = 0.5;

    m_a[2][0] = 0.0;
    m_a[2][1] = 0.0;
    m_a[2][2] = 1.0;

    m_c[0] = 0.5;
    m_c[1] = 0.5;
    m_c[2] = 1.0;
  }
};

/*
//
//  alpha = 0.5 Midpoint
//  alpha = 1 Heun
//  alpha = 2./3 Ralston
//
template < double alpha = 0.5 >\
struct RK2\
{\
  static int num_steps() noexcept  { return 2; }\

  static void initialize(std::vector<std::vector<double>>& m_a,
        std::vector<double>& m_b,std::vector<double>& m_c) noexcept
  {
    m_a = std::vector<std::vector<double>>(1);
    m_a[0] = std::vector< std::vector<double> >(1,alpha);
    m_b = std::vector<double>(2);
    m_c = std::vector<double>(1,alpha);
    double ohalf = 0.5/alpha;
    m_b[0] = 1 - ohalf;
    m_b[1] = ohalf;;
  }
};
*/

struct Midpoint
{
  static int num_steps() noexcept  { return 2; }

  static void initialize(std::vector<std::vector<double>>& m_a,
        std::vector<double>& m_b,std::vector<double>& m_c) noexcept
  {
    m_a = std::vector<std::vector<double>>(1);
    m_a[0] = std::vector<double>(1,0.5);
    m_b = std::vector<double>(2);
    m_c = std::vector<double>(1,0.5);
    m_b[0] = 0;
    m_b[1] = 1;
  }
};

struct Heun
{
  static int num_steps() noexcept  { return 2; }

  static void initialize(std::vector<std::vector<double>>& m_a,
        std::vector<double>& m_b,std::vector<double>& m_c) noexcept
  {
    m_a = std::vector<std::vector<double>>(1);
    m_a[0] = std::vector<double>(1,1.0);
    m_b = std::vector<double>(2);
    m_c = std::vector<double>(1,1.0);
    m_b[0] = 0.5;
    m_b[1] = 0.5;
  }
};

struct Ralston
{
  static int num_steps() noexcept  { return 2; }

  static void initialize(std::vector<std::vector<double>>& m_a,
        std::vector<double>& m_b,std::vector<double>& m_c) noexcept
  {
    double two3 = 2./3.;
    m_a = std::vector<std::vector<double>>(1);
    m_a[0] = std::vector<double>(1,two3);
    m_b = std::vector<double>(2);
    m_c = std::vector<double>(1,two3);
    m_b[0] = 0.25;
    m_b[1] = 0.75;
  }
};

}
}
