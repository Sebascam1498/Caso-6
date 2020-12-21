#include <vector>
#include <iostream>
#include <string>
#include <fstream>   
#include <sstream>
#include <omp.h>
#include <chrono>

using namespace std;

std::vector<vector<double>> procesarDatosBirds(std::string textoLeido){

    std::vector<vector<double>> infoPajaros{};
    std::vector<double> vectorAux{0,0,0};
    std::string indiceTxt = "";
    int largoString = textoLeido.length();
    int indice = 0;
    double num = 0;


    while (indice < largoString)
    {
        while (textoLeido.at(indice)!=*" ")
        {
            indice++;
        }
        indice++;
        while (textoLeido.at(indice)!=*" ")
        {
            indiceTxt += textoLeido.at(indice);
            indice++;
        }
        num = stod(indiceTxt);
        vectorAux[0]=num;
        indiceTxt="";
        num=0;
        indice++;
        while (textoLeido.at(indice)!=*" ")
        {
            indiceTxt += textoLeido.at(indice);
            indice++;
        }
        num = stod(indiceTxt);
        vectorAux[1]=num;
        indiceTxt="";
        num=0;
        indice++;
        while (textoLeido.at(indice)!=*"\n")
        {
            indiceTxt += textoLeido.at(indice);
            indice++;
            if (indice==largoString-1)
            {
                num = stod(indiceTxt);
                vectorAux[2]=num;
                infoPajaros.push_back(vectorAux);
                return infoPajaros;
            }
            
        }
        num = stod(indiceTxt);
        vectorAux[2]=num;
        indiceTxt="";
        num=0;
        infoPajaros.push_back(vectorAux);
        vectorAux = {0,0,0};

        
    }
    
    return infoPajaros;

}

std::vector<vector<double>> rankingPajaros(std::vector<vector<double>> vectorDatos,int indice){
    
    std::vector<vector<double>> infoPajarosFinal{};
    std::vector<double> datosPajaros{0,0,0.0};
    int id=0;
    int velocidad = 0;
    int flagVMax = 0;
    int flagVMin = 0;
    int flagDMax = 0;
    int flagDMin = 0;
    std::vector<vector<double>> maxDistancia{{0,0.0},{0,0.0}};
    std::vector<vector<double>> minDistancia{{0,99999.0},{0,99999.0}};
    std::vector<vector<double>> rapida{{0,0},{0,0}};
    std::vector<vector<double>> lenta{{0,99999.0},{0,99999.0}};


    int contadorVD = vectorDatos.size();

    for (int i = 0; i < indice; i++)
    {
        datosPajaros[0]=i;
        infoPajarosFinal.push_back (datosPajaros);
    }
    
    //Se paraleliza el for para sacar el ranking
    #pragma omp parallel for
    for (int i = 0; i < contadorVD; i++)
    {
        id=vectorDatos[i][0];
        velocidad=vectorDatos[i][1];
        infoPajarosFinal[id][2]+=vectorDatos[i][2];
        if (infoPajarosFinal[id][1]<velocidad)
        {
            infoPajarosFinal[id][1]=velocidad;
        }
        // Ver velocidad Maxima
        if ((infoPajarosFinal[id][1]>rapida[0][1]) && (flagVMax == 0))
        {
            rapida[0][0]=infoPajarosFinal[id][0];
            rapida[0][1]=infoPajarosFinal[id][1];
            flagVMax=1;
        }
        if ((infoPajarosFinal[id][2]>rapida[1][1]) && (flagVMax == 0))
        {
            rapida[1][0]=infoPajarosFinal[id][0];
            rapida[1][1]=infoPajarosFinal[id][1];
            flagVMax=1;
        }
        // Ver velocidad Minima
        if ((infoPajarosFinal[id][1]<lenta[0][1]) && (flagVMin == 0))
        {
            lenta[0][0]=infoPajarosFinal[id][0];
            lenta[0][1]=infoPajarosFinal[id][1];
            flagVMin=1;
        }
        if ((infoPajarosFinal[id][1]<lenta[1][1]) && (flagVMin == 0))
        {
            lenta[1][0]=infoPajarosFinal[id][0];
            lenta[1][1]=infoPajarosFinal[id][1];
            flagVMin=1;
        }
        // Ver distancia Maxima
        if ((infoPajarosFinal[id][2]>maxDistancia[0][1]) && (flagDMax == 0))
        {
            maxDistancia[0][0]=infoPajarosFinal[id][0];
            maxDistancia[0][1]=infoPajarosFinal[id][2];
            flagDMax=1;
        }
        if ((infoPajarosFinal[id][2]>maxDistancia[1][1]) && (flagDMax == 0))
        {
            maxDistancia[1][0]=infoPajarosFinal[id][0];
            maxDistancia[1][1]=infoPajarosFinal[id][2];
            flagDMax=1;
        }
        // Ver distancia minima
        
        flagVMax=0;
        flagVMin=0;
        flagDMax=0;
    }
    // Queda afuera del for debido a que hasta el final se conoce la distancia mas corta que se ha recorrido
    #pragma omp parallel for
    for (int i = 0; i < infoPajarosFinal.size(); i++)
    {
        if ((infoPajarosFinal[i][2]<minDistancia[0][1]) && (flagDMin == 0))
        {
            minDistancia[0][0]=infoPajarosFinal[i][0];
            minDistancia[0][1]=infoPajarosFinal[i][2];
            flagDMin=1;
        }
        if ((infoPajarosFinal[i][2]<minDistancia[1][1]) && (flagDMin == 0))
        {
            minDistancia[1][0]=infoPajarosFinal[i][0];
            minDistancia[1][1]=infoPajarosFinal[i][2];
            flagDMin=1;
        }
        flagDMin=0;

    }
    
    cout<<"["<<lenta[0][0]<<","<<lenta[0][1]<<","<<"0x00f91d"<<"],";
    cout<<"["<<lenta[1][0]<<","<<lenta[1][1]<<","<<"0x00f9e6"<<"],";
    cout<<"["<<rapida[0][0]<<","<<rapida[0][1]<<","<<"0x0004f9"<<"],";
    cout<<"["<<rapida[1][0]<<","<<rapida[1][1]<<","<<"0xf90000"<<"],";
    cout<<"["<<maxDistancia[0][0]<<","<<maxDistancia[0][1]<<","<<"0x000000"<<"],";
    cout<<"["<<maxDistancia[1][0]<<","<<maxDistancia[1][1]<<","<<"0x7e7e7e"<<"],";
    cout<<"["<<minDistancia[0][0]<<","<<minDistancia[0][1]<<","<<"0xf600ec"<<"],";
    cout<<"["<<minDistancia[1][0]<<","<<minDistancia[1][1]<<","<<"0xf0ff00"<<"]";
    //cout<<"["<<lenta[0][0]<<","<<lenta[0][1]<<","<<"00f91d"<<"]";
    return infoPajarosFinal;

}



int main(){

    std::ifstream inFile;
    inFile.open("DatosPajaros.txt");

    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string textoLeido = buffer.str();
    int largo = textoLeido.length();
    std::vector<vector<double>> infoPajaros{};
    std::vector<vector<double>> infoPajarosRanking{};
    infoPajaros=procesarDatosBirds(textoLeido);
    infoPajarosRanking=rankingPajaros(infoPajaros,15);
  
    


}