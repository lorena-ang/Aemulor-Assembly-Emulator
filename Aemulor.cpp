//
//  main.cpp
//  Emulador
//
//  Created by Lore Ang on 6/28/19.
//  Copyright © 2019 Lore Ang. All rights reserved.
//

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
using namespace std;

// Función que da el int obtenido del arreglo de chars con la instruccion (['L', 'D', 'A', ' ', 'A', ',', ' ', '0', '0', '1'] = 001)
int charANum(char cArrA[])
{
    // Declaración de variables
    int iN, iN1, iN2, iN3;
    
    if (cArrA[0] == 'O' && cArrA[1] == 'R' && cArrA[2] == 'G')
    {
        iN1 = (int)cArrA[5] - 48;
        iN2 = (int)cArrA[6] - 48;
        iN3 = (int)cArrA[7] - 48;
        iN = (iN1 * 100) + (iN2 * 10) + iN3;
    }
    else
    {
        iN1 = (int)cArrA[7] - 48;
        iN2 = (int)cArrA[8] - 48;
        iN3 = (int)cArrA[9] - 48;
        iN = (iN1 * 100) + (iN2 * 10) + iN3;
    }
    
    return iN;
}

// Función que convierte cada caracter del string en elemento de un arreglo de chars ("LDA A, 001" = ['L', 'D', 'A', ' ', 'A', ',', ' ', '0', '0', '1'])
void stringArreglo(string sDato, char cArrA[])
{
    for (int iN = 0; iN < sDato.size(); iN++)
    {
        cArrA[iN] = sDato[iN];
    }
}

// Función que obtiene los 3 dígitos del número del origen ("ORG, 001" = 1)
int origen(string sOrigen, char cArrA[])
{
    // Declaración de variables
    int iN;
    
    // Convertir ORG, 000 a arreglo de chars
    stringArreglo(sOrigen, cArrA);
    
    // Calcular el origen en int
    iN = charANum(cArrA);
    
    return iN;
}

// Función que despliega todos los valores del ciclo fetch
void desplegarFetch(int iMAR, int iPC, int iAC, string sIR, string sMDR, string sFR)
{
    cout << "\nCiclo FETCH";
    cout << "\nPC = " << iPC;
    cout << "\nIR = " << sIR;
    cout << "\nMDR = " << sMDR;
    cout << "\nMAR = " << iMAR;
    cout << "\nFR = " << sFR;
    if (iAC < -999999)
    {
        cout << "\nAC = ";
    }
    else
    {
        cout << "\nAC = " << iAC;
    }
}

// Función que despliega todos los valores del ciclo execute
void desplegarExecute(int iMAR, int iPC, int iPCF, int iAC, int iMDR, string sMDR, string sIR, string sFR, bool bMDR)
{
    cout << "\n\nCiclo EXECUTE";
    if (iPCF > iPC)
    {
        cout << "\nPC = " << iPCF;
    }
    else
    {
       cout << "\nPC = " << iPC;
    }
    cout << "\nIR = " << sIR;
    if (bMDR)
    {
        cout << "\nMDR = " << sMDR;
    }
    else
    {
        cout << "\nMDR = " << iMDR;
    }
    cout << "\nMAR = " << iMAR;
    cout << "\nFR = " << sFR;
    cout << "\nAC = " << iAC << endl;
}

// Función que inicializa todos los valores del arreglo iMM con 9999999
void inicializarMM(int iMM[])
{
    for (int i = 0; i < 1000; i++)
    {
        iMM[i] = 9999999;
    }
}

// Función que despliega los datos de memoria que sí cambiaron al finalizar el programa
void desplegarMemoria(int iMM[])
{
    cout << "\n\nMM - Valores\n";
    
    for (int i = 0; i < 999; i++)
    {
        if (iMM[i] < 9999999)
        {
            cout << "Dirección " << i << " = " << iMM[i] << endl;
        }
    }
}

// Función donde se llevan a cabo todos los procesos de fetch y execute y que despliega los valores
void execute(string sProg[], string sOrigen, int iLen, int iMM[])
{
    // Declaración de variables
    string sDato;
    char cArrO[8];
    int iMV[1], iMAR, iPC, iAC, iMDR, iPCF;
    bool bZ = false, bN = false, bV = false, bSeguir = false;
    
    // Calcular origen con función
    int iOrigen = origen(sOrigen, cArrO);
    
    string sIR, sMDR, sFR = " ";
    
    // Ciclo for para calcular valores de FETCH
    for (iPC = iOrigen; iPC <= iLen + iOrigen && !bSeguir; iPC++)
    {
        bool bMDR = false;
        
        if (iPC <= iLen + iOrigen)
        {
            iMAR = iPC;
            sMDR = sProg[iPC];
            sIR = sMDR;
        }
        
        desplegarFetch(iMAR, iPC, iAC, sIR, sMDR, sFR);
        
        // Asignar cada instrucción como un elemento del array del programa
        sDato = sProg[iPC];
        
        // Array de chars que contendrá cada caracter de la instrucción (ex. LDA A, 000)
        char cArrA[sDato.size()];
        stringArreglo(sDato, cArrA);
        
        // Pasar el número de la instrucción a int que represente IR(2-0)
        int iIR = charANum(cArrA);
        
        if (cArrA[0] == 'C' && cArrA[1] == 'L' && cArrA[2] == 'A')
        {
            iAC = 0;
            bMDR = true;
        }
        else if (cArrA[0] == 'L' && cArrA[1] == 'D' && cArrA[2] == 'A')
        {
            switch (cArrA[4])
            {
                case 'A':
                    iMAR = iIR;
                    if (iMM[iMAR] == 9999999)
                    {
                        iMDR = iMV[0];
                    }
                    else
                    {
                        iMDR = iMM[iMAR];
                    }
                    iAC = iMDR;
                    break;
                case 'I':
                    iAC = iIR;
                    bMDR = true;
                    break;
                case 'R':
                    iMAR = iPC + iIR;
                    if (iMM[iMAR] == 9999999)
                    {
                        iMDR = iMV[0];
                    }
                    else
                    {
                        iMDR = iMM[iMAR];
                    }
                    iAC = iMDR;
                    break;
                case 'D':
                    iMAR = iIR;
                    iMAR = iMM[iMAR];
                    iMDR = iMM[iMAR];
                    iAC = iMDR;
                    break;
            }
        }
        else if (cArrA[0] == 'S' && cArrA[1] == 'T' && cArrA[2] == 'A')
        {
            switch (cArrA[4])
            {
                case 'A':
                    iMAR = iIR;
                    iMDR = iAC;
                    iMM[iMAR] = iMDR;
                    break;
                case 'I':
                    cout << "Error: Direccionamiento no permitido\n";
                    break;
                case 'R':
                    iMAR = iPC + iIR;
                    iMDR = iAC;
                    iMM[iMAR] = iMDR;
                    break;
                case 'D':
                    iMAR = iIR;
                    iMAR = iMM[iMAR];
                    iMDR = iAC;
                    iMM[iMAR] = iMDR;
                    break;
            }
        }
        else if (cArrA[0] == 'A' && cArrA[1] == 'D' && cArrA[2] == 'D')
        {
            switch (cArrA[4])
            {
                case 'A':
                    iMAR = iIR;
                    iMDR = iMM[iMAR];
                    iAC += iMDR;
                    bMDR = true;
                    break;
                case 'I':
                    iAC += iIR;
                    bMDR = true;
                    break;
                case 'R':
                    iMAR = iPC + iIR;
                    iMDR = iMM[iMAR];
                    iAC += iMDR;
                    break;
                case 'D':
                    iMAR = iIR;
                    iMAR = iMM[iMAR];
                    iMDR = iMM[iMAR];
                    iAC += iMDR;
                    break;
            }
        }
        else if (cArrA[0] == 'S' && cArrA[1] == 'U' && cArrA[2] == 'B')
        {
            switch (cArrA[4])
            {
                case 'A':
                    iMAR = iIR;
                    iMDR = iMM[iMAR];
                    iAC -= iMDR;
                    break;
                case 'I':
                    iAC -= iIR;
                    bMDR = true;
                    break;
                case 'R':
                    iMAR = iPC + iIR;
                    iMDR = iMM[iMAR];
                    iAC -= iMDR;
                    break;
                case 'D':
                    iMAR = iIR;
                    iMAR = iMM[iMAR];
                    iMDR = iMM[iMAR];
                    iAC -= iMDR;
                    break;
            }
        }
        else if (cArrA[0] == 'N' && cArrA[1] == 'E' && cArrA[2] == 'G')
        {
            iAC = -iAC;
            bMDR = true;
        }
        else if (cArrA[0] == 'N' && cArrA[1] == 'O' && cArrA[2] == 'P')
        {
            using namespace std::this_thread; // sleep_for, sleep_until
            using namespace std::chrono; // nanoseconds, system_clock, seconds
            
            sleep_for(seconds(1));
        }
        else if (cArrA[0] == 'J' && cArrA[1] == 'M' && cArrA[2] == 'P')
        {
            switch (cArrA[4])
            {
                case 'A':
                    iPCF = iIR;
                    iPC = iIR - 1;
                    bMDR = true;
                    break;
                case 'I':
                    cout << "Error: Direccionamiento no permitido\n";
                    break;
                case 'R':
                    iMAR = iPC + iIR;
                    iMDR = iMM[iMAR];
                    iPC = iMDR;
                    break;
                case 'D':
                    iMAR = iIR;
                    iMAR = iMM[iMAR];
                    iMDR = iMM[iMAR];
                    iPC = iMDR;
                    break;
            }
            
            if (bZ && iAC == 0)
            {
                sFR = "Z";
            }
            else if (bN && iAC < 0)
            {
                sFR = "N";
            }
            else if (bV && iAC > 999999)
            {
                sFR = "V";
            }
        }
        else if (cArrA[0] == 'J' && cArrA[1] == 'M' && cArrA[2] == 'Z')
        {
            switch (cArrA[4])
            {
                case 'A':
                    if (sFR == "Z")
                    {
                        iPCF = iIR;
                        iPC = iIR - 1;
                    }
                    break;
                case 'I':
                    cout << "Error: Direccionamiento no permitido\n";
                    break;
                case 'R':
                    if (sFR == "Z")
                    {
                        iMAR = iPC + iIR;
                        iMDR = iMM[iMAR];
                        iPC = iMDR;
                    }
                    break;
                case 'D':
                    if (sFR == "Z")
                    {
                        iMAR = iIR;
                        iMAR = iMM[iMAR];
                        iMDR = iMM[iMAR];
                        iPC = iMDR;
                    }
                    break;
            }
            bZ = true;
            bMDR = true;
        }
        else if (cArrA[0] == 'J' && cArrA[1] == 'M' && cArrA[2] == 'N')
        {
            switch (cArrA[4])
            {
                case 'A':
                    if (sFR == "N")
                    {
                        iPCF = iIR;
                        iPC = iIR - 1;
                    }
                    break;
                case 'I':
                    cout << "Error: Direccionamiento no permitido\n";
                    break;
                case 'R':
                    if (sFR == "N")
                    {
                        iMAR = iPC + iIR;
                        iMDR = iMM[iMAR];
                        iPC = iMDR;
                    }
                    break;
                case 'D':
                    if (sFR == "N")
                    {
                        iMAR = iIR;
                        iMAR = iMM[iMAR];
                        iMDR = iMM[iMAR];
                        iPC = iMDR;
                    }
                    break;
            }
            bN = true;
        }
        else if (cArrA[0] == 'J' && cArrA[1] == 'M' && cArrA[2] == 'V')
        {
            switch (cArrA[4])
            {
                case 'A':
                    if (sFR == "V")
                    {
                        iPCF = iIR;
                        iPC = iIR - 1;
                    }
                    break;
                case 'I':
                    cout << "Error: Direccionamiento no permitido\n";
                    break;
                case 'R':
                    if (sFR == "V")
                    {
                        iMAR = iPC + iIR;
                        iMDR = iMM[iMAR];
                        iPC = iMDR;
                    }
                    break;
                case 'D':
                    if (sFR == "V")
                    {
                        iMAR = iIR;
                        iMAR = iMM[iMAR];
                        iMDR = iMM[iMAR];
                        iPC = iMDR;
                    }
                    break;
            }
            bV = true;
        }
        
        if (cArrA[0] == 'H' && cArrA[1] == 'L' && cArrA[2] == 'T')
        {
            bSeguir = true;
        }
        else
        {
            desplegarExecute(iMAR, iPC, iPCF, iAC, iMDR, sMDR, sIR, sFR, bMDR);
        }
    }
}

int main()
{
    // Declaración de variables
    int iLen;
    int iMM[999];
    string sOrigen, sInst;
    char cArrO[8];
    
    cout << "Bienvenido a este emulador.\n";
    
    cout << "\nIngresa el dato de origen en el formato 'ORG, XXX' considerando que los espacios de memoria en este programa van de 000-999: ";
    getline(cin, sOrigen);
    cout << "\nIngresa el número de líneas de tu programa: ";
    cin >> iLen;
    
    inicializarMM(iMM);
    
    // Calcular origen en int con función y asignar a una vcc
    int iOrigen = origen(sOrigen, cArrO);
    
    // Declaración de arreglo que contendrá el programa entero
    string sProg[iLen + iOrigen];
    
    for (int i = iOrigen; i <= iLen + iOrigen - 1; i++)
    {
        if (i < 10)
        {
            cout << "Línea 00" << i << ": ";
            if (i == iOrigen)
            {
                cin.ignore();
            }
            getline(cin, sInst);
            sProg[i] = sInst;
        }
        else if (i >= 10 && i < 100)
        {
            cout << "Línea 0" << i << ": ";
            getline(cin, sInst);
            sProg[i] = sInst;
        }
        else if (i >= 100 && i < 1000)
        {
            cout << "Línea " << i << ": ";
            getline(cin, sInst);
            sProg[i] = sInst;
        }
    }
    
    cout << "Enseguida comenzarán a desplegarse los ciclos FETCH y EXECUTE.\n";
    execute(sProg, sOrigen, iLen, iMM);

    desplegarMemoria(iMM);
    
    cout << "\n¡Gracias por usar este emulador!\n";
    
    return 0;
}
