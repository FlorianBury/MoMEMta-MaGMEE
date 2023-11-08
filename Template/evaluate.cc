/*
 *  MoMEMta: a modular implementation of the Matrix Element Method
 *  Copyright (C) 2016  Universite catholique de Louvain (UCL), Belgium
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <momemta/MoMEMta.h>
#include <stdio.h>

#include <%(process_class)s.h>

/*
 * Executable that evaluates a ME for a set of particles provided as input 
 * first 8 alues are q1 and q2 p4 (in order E,px,py,pz)
 * next values are groups of 5 values (pdgid,E,px,py,pz) for each final state 
 */

using namespace %(namespace)s;

int main(int argc, char** argv) {

    printf("You have entered %%d arguments:\n", argc-1);
    int n = argc - 9; // [0] is script
    if (n%%5 != 0. || n < 0){
        printf("8 args for q1 and q2, remaining %%d for final states which is not a multiple of 5 (pdg id + 4-vector), will stop here",n);
        return 1;
    }
    n = n/5; // number of particles

    std::vector<double> q1 
    {
        std::atof(argv[1]),
        std::atof(argv[2]),
        std::atof(argv[3]),
        std::atof(argv[4])
    };
    std::vector<double> q2 
    {   
        std::atof(argv[5]),
        std::atof(argv[6]),
        std::atof(argv[7]),
        std::atof(argv[8])
    };
    auto initialMomenta = std::make_pair(q1,q2);
    printf("q1 : E=%%f, px=%%f, py=%%f, pz=%%f\n",q1[0],q1[1],q1[2],q1[3]);
    printf("q1 : E=%%f, px=%%f, py=%%f, pz=%%f\n",q2[0],q2[1],q2[2],q2[3]);

    std::vector<std::pair<int,std::vector<double>>> finalState;
    for (int i=0; i<n; i++){
        int j = i * 5;
        int pdgid = std::atoi(argv[9+j]);
        std::vector<double> vec 
        {
            std::atof(argv[10+j]),
            std::atof(argv[11+j]),
            std::atof(argv[12+j]),
            std::atof(argv[13+j])
        };
        finalState.push_back(std::make_pair(pdgid,vec));
        printf("Part %%d : id=%%d E=%%f, px=%%f, py=%%f, pz=%%f\n",i,pdgid,vec[0],vec[1],vec[2],vec[3]);
    }

    ParameterSet lua_parameters;
    lua_parameters.set("card","%(card)s");

    %(process_class)s m_ME = %(process_class)s(lua_parameters);

    m_ME.resetHelicities();

    auto result = m_ME.compute(initialMomenta,finalState);
    for (auto const& r : result){
        std::cout<<"PDG ID "<<r.first.first<<" PDG ID "<<r.first.second<<" ME = "<<r.second<<std::endl; 
    }

    return 0;
}
