const fetchLog = document.getElementById('fetchLog');
const input = document.getElementById('textArea');
const sOrigen = prompt('Bienvenido a este emulador.\nIngresa el dato de origen en el formato ORG, XXX considerando que los espacios de memoria en este programa van de 000-999:');
var iMAR, iPC, iAC = undefined, iMDR, iPCF, iOrigen;
var sIR, sMDR, sFR = " ";
var bZ = false, bN = false, bV = false, bSeguir = false;

$('#execute').click(function(){
    click();
});

// Función que da el int obtenido del arreglo de chars con la instruccion (['L', 'D', 'A', ' ', 'A', ',', ' ', '0', '0', '1'] = 001)
function charANum(cArrA)
{
    // Retorna últimos 3 elementos del arreglo convertidos a int
    return parseInt(`${cArrA[7]}${cArrA[8]}${cArrA[9]}`);
}

// Función que convierte cada dígito del string en un arreglo de chars ("LDA A, 001" = ['L', 'D', 'A', ' ', 'A', ',', ' ', '0', '0', '1'])
function stringArreglo(sDato)
{
    var cArrA = [];
    
    for (var iN = 0; iN < sDato.length; iN++)
    {
        cArrA[iN] = sDato[iN];
    }

    return cArrA;
}

// Función que obtiene los 3 dígitos del número del origen ("ORG, 001" = "001")
function origen(sOrigen)
{   
    return sOrigen.substr(5, 3);
}

// Función que despliega todos los valores del ciclo fetch
function desplegarFetch(iMAR, iPC, iAC, sIR, sMDR, sFR)
{
    $('#fetchLog').append(`
        <p>FETCH</p>
        <ul>
            <li>PC = ${iPC}</li>
            <li>IR = ${sIR}</li>
            <li>MDR = ${sMDR}</li>
            <li>MAR = ${iMAR}</li>
            <li>FR = ${sFR}</li>
            <li>AC = ${iAC < -999999 ? " " : iAC}</li>
        </ul>
    `);
}

// Función que despliega todos los valores del ciclo execute
function desplegarExecute(iMAR, iPC, iPCF, iAC, iMDR, sMDR, sIR, sFR, bMDR)
{
    $('#fetchLog').append(`
        <p>EXECUTE</p>
        <ul>
            <li>PC = ${iPCF > iPC ? iPCF : iPC}</li>
            <li>IR = ${sIR}</li>
            <li>MDR = ${bMDR ? sMDR : iMDR}</li>
            <li>MAR = ${iMAR}</li>
            <li>FR = ${sFR}</li>
            <li>AC = ${iAC}</li>
        </ul>
    `);
}

// Función que despliega los datos de memoria que sí cambiaron al finalizar el programa    
function desplegarMemoria(iMM)
{
    for (var i = 0; i < 999; i++)
    {
        if (iMM[i] < 9999999)
        {
            $('#memoriaLog').append(`
                <ul>
                    <li>Dirección ${i} = ${iMM[i]}</li>
                </ul>
            `);
        }
    }
}

function execute(sProg, sOrigen, iLen, iMM)
{
    $('#fetchLog').empty()
    // Declaración de variables
    var sDato;
    var cArrO = [];
    
    // Calcular origen con función
    var iOrigen = origen(sOrigen);
    iOrigen = parseInt(iOrigen);
    
    // Calcular cantidad de líneas escritas por usuario
    iLen = sProg.length;
    
    // Ciclo for para calcular valores de FETCH
    for (iPC = iOrigen; iPC < iLen + iOrigen && !bSeguir; iPC++)
    {
        var bMDR = false;
        
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
        var cArrA = stringArreglo(sDato);
        
        // Pasar el número de la instrucción a int que represente IR(2-0)
        var iIR = charANum(cArrA);
        
        if (cArrA[0] == 'C' && cArrA[1] == 'L' && cArrA[2] == 'A')
        {
            if (cArrA.length > 3)
            {
                alert("¡Error, recuerda que CLA no tiene ningún direccionamiento! Favor de corregir, refrescar la página y volver a intentar.");
            }
            else
            {
                iAC = 0;
                bMDR = true;
            }
        }
        else if (cArrA[0] == 'L' && cArrA[1] == 'D' && cArrA[2] == 'A')
        {
            switch (cArrA[4])
            {
                case 'A':
                    iMAR = iIR;
                    iMDR = iMM[iMAR] == undefined ? null : iMM[iMAR];
                    iAC = iMDR == undefined ? null : iMDR;
                    break;
                case 'I':
                    iAC = iIR;
                    bMDR = true;
                    break;
                case 'R':
                    iMAR = iPC + iIR;
                    iMDR = iMM[iMAR] == undefined ? null : iMM[iMAR];
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
                default:
                    alert("¡Error, has ingresado un direccionamiento no permitido! Favor de corregir, refrescar la página y volver a intentar.");
            }
        }
        else if (cArrA[0] == 'A' && cArrA[1] == 'D' && cArrA[2] == 'D')
        {
            switch (cArrA[4])
            {
                case 'A':
                    iMAR = iIR;
                    iMDR = iMM[iMAR];
                    iAC += iMDR == undefined ? null : iMDR;
                    bMDR = true;
                    break;
                case 'I':
                    iAC += iIR;
                    bMDR = true;
                    break;
                case 'R':
                    iMAR = iPC + iIR;
                    iMDR = iMM[iMAR];
                    iAC += iMDR == undefined ? null : iMDR;
                    break;
                case 'D':
                    iMAR = iIR;
                    iMAR = iMM[iMAR];
                    iMDR = iMM[iMAR];
                    iAC += iMDR == undefined ? null : iMDR;
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
                    iAC -= iMDR == undefined ? null : iMDR;
                    break;
                case 'I':
                    iAC -= iIR;
                    bMDR = true;
                    break;
                case 'R':
                    iMAR = iPC + iIR;
                    iMDR = iMM[iMAR];
                    iAC -= iMDR == undefined ? null : iMDR;
                    break;
                case 'D':
                    iMAR = iIR;
                    iMAR = iMM[iMAR];
                    iMDR = iMM[iMAR];
                    iAC -= iMDR == undefined ? null : iMDR;
                    break;
            }
        }
        else if (cArrA[0] == 'N' && cArrA[1] == 'E' && cArrA[2] == 'G')
        {
            if (cArrA.length > 3)
            {
                alert("¡Error, recuerda que NEG no tiene ningún direccionamiento! Favor de corregir, refrescar la página y volver a intentar.");
            }
            else
            {
                iAC = -iAC;
                bMDR = true;
            }
        }
        else if (cArrA[0] == 'N' && cArrA[1] == 'O' && cArrA[2] == 'P')
        {
            if (cArrA.length > 3)
            {
                alert("¡Error, recuerda que NOP no tiene ningún direccionamiento! Favor de corregir, refrescar la página y volver a intentar.");
            }
            else
            {
                function delayNOP(i) 
                {
                    setTimeout(function(){
                        i = 1;
                    }, i * 1000);
                }
            }
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
                default:
                    alert("¡Error, has ingresado un direccionamiento no permitido! Favor de corregir, refrescar la página y volver a intentar.");
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
                default:
                    alert("¡Error, has ingresado un direccionamiento no permitido! Favor de corregir, refrescar la página y volver a intentar.");
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
                default:
                    alert("¡Error, has ingresado un direccionamiento no permitido! Favor de corregir, refrescar la página y volver a intentar.");
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
                default:
                    alert("¡Error, has ingresado un direccionamiento no permitido! Favor de corregir, refrescar la página y volver a intentar.");
            }
            bV = true;
        }

        if (cArrA[0] == 'H' && cArrA[1] == 'L' && cArrA[2] == 'T' && cArrA.length > 3)
        {
            alert("¡Error, recuerda que HLT no tiene ningún direccionamiento! Favor de corregir, refrescar la página y volver a intentar.");
        }
        else if (cArrA[0] == 'H' && cArrA[1] == 'L' && cArrA[2] == 'T')
        {
            bSeguir = true;
        }
        else
        {
            desplegarExecute(iMAR, iPC, iPCF, iAC, iMDR, sMDR, sIR, sFR, bMDR);
        }
    }
}

function click() // ex main
{
    // Declaración de variables
    var iLen;
    var iMM = [];
    
    // Calcular origen en int con función y asignar a una vcc
    iOrigen = origen(sOrigen);
    iOrigen = parseInt(iOrigen);
    
    // Declaración de arreglo que contendrá el programa entero
    var sProg = [];
    
    // Separar cada línea ingresada por usuario y guardar cada instrucción en un arreglo de strings 
    var lines = $('textarea').val().split('\n');
    for (var i = 0; i < lines.length; i++) {
        sProg[i + iOrigen] = lines[i];
    }
    
    // Llamar a función que realiza y despliega los registros tras realizar ciclos fetch y execute
    execute(sProg, sOrigen, iLen, iMM);

    // Despliega el valor de los espacios de memoria que el usuario utilizó
    desplegarMemoria(iMM);
}