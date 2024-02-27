#pragma once
#include <vector>
namespace petlib {
namespace runge_kutta {

struct RK4Fehlberg {
public:
  static int num_steps() noexcept { return 6; }

  static int q() noexcept { return 4;}

  static void initialize(
    std::vector<std::vector<double> >& a,
    std::vector<double>& c,
    std::vector<double>& b,
    std::vector<double>& berr) noexcept
  {
    b = std::move(std::vector<double>(6));
    c = std::move(std::vector<double>(5));
    berr = std::move(std::vector<double>(6));
    a = std::move(std::vector<std::vector<double>>(5));
    for (int i=0;i<5;++i) a[i] = std::vector<double>((i+1));

    b[0] = 16. / 135.;
    b[1] = 0.0;
    b[2] = 6656. / 12825.;
    b[3] = 28561. / 56430.;
    b[4] = -9. / 50.0;
    b[5] = 2. / 55.0;

    berr[0] = b[0] - 25.0 / 216.0;
    berr[1] = 0.0;
    berr[2] = b[2] - 1408. / 2565.;
    berr[3] = b[3] - 2197.0 / 4104.;
    berr[4] = b[4] + 0.2;
    berr[5] = -b[5];

    a[0][0] = 0.25;

    a[1][0] = 3. / 32.;
    a[1][1] = 9. / 32.;

    a[2][0] = 1932. / 2197.0;
    a[2][1] = -7200. / 2197.0;
    a[2][2] = 7296. / 2197.0;

    a[3][0] = 439. / 216.;
    a[3][1] = -8.0;
    a[3][2] = 3680. / 513.;
    a[3][3] = -845.0 / 4104.;

    a[4][0] = -8. / 27.0;
    a[4][1] = 2.0;
    a[4][2] = -3544. / 2565.;
    a[4][3] = 1859. / 4104.;
    a[4][4] = -11. / 40.;

    c[0] = 0.25;
    c[1] = 3. / 8.;
    c[2] = 12. / 13.;
    c[3] = 1.0;
    c[4] = 0.5;
  }
};

struct HeunEuler {
public:
  static int num_steps() noexcept { return 2; }

  static int q() noexcept { return 1;}

  static void initialize(
    std::vector<std::vector<double> >& a,
    std::vector<double>& c,
    std::vector<double>& b,
    std::vector<double>& berr) noexcept
  {
    b = std::move(std::vector<double>(2));
    c = std::move(std::vector<double>(1));
    berr = std::move(std::vector<double>(1));
    a = std::move(std::vector<std::vector<double>>(1));
    a[0] = std::vector<double>(1);
    b[0] = 0.5;
    b[1] = 0.5;
    berr[0] = b[0] - 1.0;
    berr[1] = b[1];
    a[0][0] = 1.0;
    c[0] = 1.0;
  }
};

/*
struct BogackiShampine {
  double b[4];
  double berr[4];
  double a[3][3];
  double c[3];  

  BogackiShampine() {
    b[0] = 2.0 / 9.;
    b[1] = 1. / 3.;
    b[2] = 4. / 9.;
    b[3] = 0.0;
    berr[0] = b[0] - 7. / 24.;
    berr[1] = b[1] - 0.25;
    berr[2] = b[2] - 1. / 3.;
    berr[3] = b[3] - 0.125;
    a[0][0] = 0.5;
    a[1][0] = 0.0;
    a[1][1] = 0.75;
    a[2][0] = 2. / 9.;
    a[2][1] = 1. / 3.;
    a[2][2] = 4. / 9.;
    c[0] = 0.5;
    c[1] = 0.75;
    c[2] = 1.0;
  }
  size_t num_stages() const noexcept { return 4; }
  const double& a(size_t i,size_t j) const noexcept { return a[i-1][j];}
  const double& b(size_t i) const noexcept { return b[i];}
  const double& berr(size_t i) const noexcept { return berr[i];}
  const double& c(size_t i) const noexcept { return c[i-1];}
};
*/
}
}