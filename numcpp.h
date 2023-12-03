#pragma once

#include <vector>
#include <functional>
#include <cmath>
#include <fstream>

class numcpp {
private:

	inline const static std::pair<double, double> make_RK4_step(double x_n, double v_n, double h, const std::function<double(double, double)>& f) {

		double k1 = f(x_n, v_n);
		double k2 = f(x_n + h / 2.0, v_n + h * k1 / 2.0);
		double k3 = f(x_n + h / 2.0, v_n + h * k2 / 2.0);
		double k4 = f(x_n + h, v_n + h * k3);

		double x = x_n + h;
		double v = v_n + h * (k1 / 6.0 + k2 / 3.0 + k3 / 3.0 + k4 / 6.0);

		return { x, v };
	}


	inline const static std::pair<double, std::vector<double>> make_RK4_system_step(double x_n, std::vector<double> v_n,
		double h, const std::vector<std::function<double(double, std::vector<double>)>>& f) {

		std::vector<double> k1;
		std::vector<double> k2;
		std::vector<double> k3;
		std::vector<double> k4;

		std::vector<double> shifted;
		shifted.resize(v_n.size());

		for (size_t i = 0; i < v_n.size(); i++)
			k1.push_back(f[i](x_n, v_n));

		for (size_t i = 0; i < v_n.size(); i++)
			shifted[i] = v_n[i] + h * k1[i] / 2.0;

		for (size_t i = 0; i < v_n.size(); i++)
			k2.push_back(f[i](x_n + h / 2.0, shifted));

		for (size_t i = 0; i < v_n.size(); i++)
			shifted[i] = v_n[i] + h * k2[i] / 2.0;

		for (size_t i = 0; i < v_n.size(); i++)
			k3.push_back(f[i](x_n + h / 2.0, shifted));

		for (size_t i = 0; i < v_n.size(); i++)
			shifted[i] = v_n[i] + h * k3[i];

		for (size_t i = 0; i < v_n.size(); i++)
			k4.push_back(f[i](x_n + h, shifted));

		double x = x_n + h;
		std::vector<double> v;
		v.resize(v_n.size());

		for (size_t i = 0; i < v_n.size(); i++)
			v[i] = v_n[i] + h * (k1[i] / 6.0 + k2[i] / 3.0 + k3[i] / 3.0 + k4[i] / 6.0);

		return { x, v };
	}

public:
    static const std::string test_stl(const std::string a){
        return a;
    }
    const static std::pair<std::pair<std::vector<double>, std::vector<double>>, std::vector<std::pair<int,std::string>>>
		RK4(double x0, double xn, double u0, std::function<double(double, double)> f,
            double step = 0.01, double precision = 0.001, size_t max_step = 1000000, bool dif_step = true, double eps = 0.01) {

        std::vector<std::pair<int,std::string>> line_table;
		size_t iters = 0;
		size_t divs = 0;
		size_t doubles = 0;

        //auto u = [x0, u0](double x) { return std::exp(x - x0) * u0; }; истинное решение переписать

		std::vector<double> x_vals{ x0 };
		std::vector<double> v_vals{ u0 };

		double h = step;
        line_table.push_back(std::make_pair(0,std::to_string(iters)));
        iters++;
        line_table.push_back(std::make_pair(0,std::to_string(x0)));
        line_table.push_back(std::make_pair(0,std::to_string(h)));
        line_table.push_back(std::make_pair(0,std::to_string(u0)));
        line_table.push_back(std::make_pair(0,"-"));
        line_table.push_back(std::make_pair(0,"-"));
        line_table.push_back(std::make_pair(0,"-"));
        line_table.push_back(std::make_pair(0,std::to_string(divs)));
        line_table.push_back(std::make_pair(0,std::to_string(doubles)));
        //line_table.push_back(std::make_pair(iters,std::to_string(u(x0))));
        //line_table.push_back(std::make_pair(iters,std::to_string(abs(u(x0))-u0)));

		while (iters < max_step && h > 0.00001) {
			if (x_vals.back() + h > xn && x_vals.back() < xn - eps)
				h = xn - eps - x_vals.back();
			else if (x_vals.back() + h > xn)
				break;

            auto [x, v] = make_RK4_step(x_vals.back(), v_vals.back(), h, f);

			auto [x_half, v_half] = make_RK4_step(x_vals.back(), v_vals.back(), h / 2.0, f);
			auto [x_help, v_help] = make_RK4_step(x_half, v_half, h / 2.0, f);

			size_t p = 4;
			double S = (v_help - v) / (pow(2.0, p) - 1.0);

			//////////////////////////////////////////////////////
			if (dif_step)
			{
				if (abs(S) > precision) {
					h /= 2.0;
					++divs;
					continue;
				}
				else if (abs(S) < precision / (pow(2.0, p + 1.0))) {
					h *= 2.0;
					++doubles;
				}
			}
			//////////////////////////////////////////////////////

			double local_p = S * pow(2, p);

            line_table.push_back(std::make_pair(iters,std::to_string(iters)));
            iters++;
            line_table.push_back(std::make_pair(iters,std::to_string(x)));
            line_table.push_back(std::make_pair(iters,std::to_string(h)));
            line_table.push_back(std::make_pair(iters,std::to_string(v)));
            line_table.push_back(std::make_pair(iters,std::to_string(v_help)));
            line_table.push_back(std::make_pair(iters,std::to_string(fabs(v_help-v))));
            line_table.push_back(std::make_pair(iters,std::to_string(local_p)));
            line_table.push_back(std::make_pair(iters,std::to_string(divs)));
            line_table.push_back(std::make_pair(iters,std::to_string(doubles)));
            //line_table.push_back(std::make_pair(iters,std::to_string(u(x))));
            //line_table.push_back(std::make_pair(iters,std::to_string(abs(u(x))-v)));

			++iters;
			divs = 0;
			doubles = 0;

			x_vals.push_back(x);
			v_vals.push_back(v);
		}

        return { {x_vals, v_vals}, line_table};
	}

    const static std::pair< std::pair<std::vector<double>, std::vector<std::vector<double>>>,
    std::pair<std::vector<std::pair<int,std::string>>,std::vector<std::pair<int,std::string>>>>
        RK4_system(double x0, double xn, std::vector<double> u0, const std::vector<std::function<double(double, std::vector<double>)>>& f,double m,double k,
            double step = 0.005, double precision = 0.001, size_t max_step = 1000000, int dif_step = 0, double eps = 0.01) {

        std::vector<std::pair<int,std::string>> line_table1;
        std::vector<std::pair<int,std::string>> line_table2;

		std::vector<double> x_vals{ x0 };
		std::vector<std::vector<double>> v_vals{ u0 };
        //double C1=u0[0];
        //double C2=u0[1];
        auto u = [k,m,u0](double x){return u0[0]*cos(sqrt(k/m)*x)+u0[1]*sin(sqrt(k/m)*x);};
        auto u1 = [k,m,u0](double x){return u0[0]*(-1)*sqrt(k/m)*sin(sqrt(k/m)*x)+u0[1]*sqrt(k/m)*cos(sqrt(k/m)*x);};
		double h = step;
		size_t num_of_steps = 0;
		size_t divs_1 = 0;
		size_t doubles_1 = 0;
		size_t divs_2 = 0;
		size_t doubles_2 = 0;

        line_table1.push_back(std::make_pair(0,std::to_string(num_of_steps)));
        line_table1.push_back(std::make_pair(0,std::to_string(x0)));
        line_table1.push_back(std::make_pair(0,std::to_string(h)));
        line_table1.push_back(std::make_pair(0,std::to_string(u0[0])));
        line_table1.push_back(std::make_pair(0,"-"));
        line_table1.push_back(std::make_pair(0,"-"));
        line_table1.push_back(std::make_pair(0,"-"));
        line_table1.push_back(std::make_pair(0,std::to_string(divs_1)));
        line_table1.push_back(std::make_pair(0,std::to_string(doubles_1)));
        line_table1.push_back(std::make_pair(0,std::to_string(u(x0))));
        line_table1.push_back(std::make_pair(0,std::to_string(fabs(u(x0)-u0[0]))));

        line_table2.push_back(std::make_pair(0,std::to_string(num_of_steps)));
        line_table2.push_back(std::make_pair(0,std::to_string(x0)));
        line_table2.push_back(std::make_pair(0,std::to_string(h)));
        line_table2.push_back(std::make_pair(0,std::to_string(u0[1])));
        line_table2.push_back(std::make_pair(0,"-"));
        line_table2.push_back(std::make_pair(0,"-"));
        line_table2.push_back(std::make_pair(0,"-"));
        line_table2.push_back(std::make_pair(0,std::to_string(divs_2)));
        line_table2.push_back(std::make_pair(0,std::to_string(doubles_2)));
        line_table2.push_back(std::make_pair(0,std::to_string(u1(x0))));
        line_table2.push_back(std::make_pair(0,std::to_string(fabs(u1(x0)-u0[1]))));
        num_of_steps++;


		while (num_of_steps < max_step && h > 0.00001) {
			if (x_vals.back() + h > xn && x_vals.back() < xn - eps)
				h = xn - eps - x_vals.back();
			else if (x_vals.back() + h > xn)
				break;

			auto [x, v] = make_RK4_system_step(x_vals.back(), v_vals.back(), h, f);

			auto [x_half, v_half] = make_RK4_system_step(x_vals.back(), v_vals.back(), h / 2, f);
			auto [x_help, v_help] = make_RK4_system_step(x_half, v_half, h / 2, f);

			size_t p = 4;

			double S_1 = (v_help[0] - v[0]) / (pow(2, p) - 1);
			double S_2 = (v_help[1] - v[1]) / (pow(2, p) - 1);

			//////////////////////////////////////////////////////
			if (dif_step)
			{
				if (abs(S_1) > precision) {
					h /= 2;
					++divs_1;
					continue;
				}
				else if (abs(S_1) < precision / (pow(2, p + 1)))
				{
					h *= 2;
					++doubles_1;
				}
				if (abs(S_2) > precision) {
					h /= 2;
					++divs_2;
					continue;
				}
				else if (abs(S_2) < precision / (pow(2, p + 1)))
				{
					h *= 2;
					++doubles_2;
				}
			}
			//////////////////////////////////////////////////////

			double local_p_1 = S_1 * pow(2, p);
			double local_p_2 = S_2 * pow(2, p);

            line_table1.push_back(std::make_pair(num_of_steps,std::to_string(num_of_steps)));
            line_table1.push_back(std::make_pair(num_of_steps,std::to_string(x)));
            line_table1.push_back(std::make_pair(num_of_steps,std::to_string(h)));
            line_table1.push_back(std::make_pair(num_of_steps,std::to_string(v[0])));
            line_table1.push_back(std::make_pair(num_of_steps,std::to_string(v_help[0])));
            line_table1.push_back(std::make_pair(num_of_steps,std::to_string(fabs(v_help[0]-v[0]))));
            line_table1.push_back(std::make_pair(num_of_steps,std::to_string(local_p_1)));
            line_table1.push_back(std::make_pair(num_of_steps,std::to_string(divs_1)));
            line_table1.push_back(std::make_pair(num_of_steps,std::to_string(doubles_1)));
            line_table1.push_back(std::make_pair(num_of_steps,std::to_string(u(x))));
            line_table1.push_back(std::make_pair(num_of_steps,std::to_string(fabs(u(x)-v[0]))));


            line_table2.push_back(std::make_pair(num_of_steps,std::to_string(num_of_steps)));
            line_table2.push_back(std::make_pair(num_of_steps,std::to_string(x)));
            line_table2.push_back(std::make_pair(num_of_steps,std::to_string(h)));
            line_table2.push_back(std::make_pair(num_of_steps,std::to_string(v[1])));
            line_table2.push_back(std::make_pair(num_of_steps,std::to_string(v_help[1])));
            line_table2.push_back(std::make_pair(num_of_steps,std::to_string(fabs(v_help[1]-v[1]))));
            line_table2.push_back(std::make_pair(num_of_steps,std::to_string(local_p_2)));
            line_table2.push_back(std::make_pair(num_of_steps,std::to_string(divs_2)));
            line_table2.push_back(std::make_pair(num_of_steps,std::to_string(doubles_2)));
            line_table2.push_back(std::make_pair(num_of_steps,std::to_string(u1(x))));
            double test1=u1(x);
            double test2=v[1];
            double test3=u1(x)-v[1];
            double test4=fabs(u1(x)-v[1]);
            line_table2.push_back(std::make_pair(num_of_steps,std::to_string(fabs(u1(x)-v[1]))));



			x_vals.push_back(x);
			v_vals.push_back(v);
			++num_of_steps;
			divs_1 = 0;
			doubles_1 = 0;
			divs_2 = 0;
			doubles_2 = 0;
		}

        return { {x_vals, v_vals},{line_table1,line_table2} };
        //v_vals это вектор векторов
	}

};
