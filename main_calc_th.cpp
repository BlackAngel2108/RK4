#include "main_calc_th.h"
#include "numcpp.h"

void MainCalcTh::setInputData(InputData *inData, InputFunc *inFunc)
{
    inD = inData;
    inF = inFunc;
}

std::pair<std::pair<std::vector<double>, std::vector<std::vector<double> > >, std::pair<std::vector<std::pair<int, std::string> >, std::vector<std::pair<int, std::string> > > > &MainCalcTh::get_answer()
{
    return answer;
}

void MainCalcTh::run()
{
    answer = numcpp::RK4_system(inD->x0, inD->xT, inD->iv,
                                { inF->f1, inF->f2 },
                                inD->h0, inD->precision,
                                inD->Nmax, inD->dif_step,  inD->epsilon); //с контролем шага эта функция глушит программу
    emit resultReady();
}
