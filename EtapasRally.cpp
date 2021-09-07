/*
    Comentarios: Según lo solicitado en el instructivo :
                    -Se obtienen los datos a partir de el archivo EtapasRally.txt de una muestra realizada por el grupo.
                    -Se emiten en el archivo ListadoRally.txt generado en el algoritmo mediante la función freopen() los listados siguientes:
                                -Listado de Corredores de Etapas y Tipos de Vehículos.
                                -Listado de Largada.
                                -Listado de Puestos Finales Carrera Rally.
                                -Listados de Ganadores por tipos de Vehiculos.
*/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>
#include <conio.h>
#include <iomanip.h>

using namespace std;

const MaxReg = 500;
const MaxTipos = 4;

typedef char str21[22];
typedef char str16[17];

struct sEtapasRally
{
	short   Etapa,
            TipoVehic,
            NumVehic;
	str21   NomPiloto,
            NomCoPiloto;
	str16   MarcaVehic;
	short   TiempoMins;
};

struct sTiemposTotales
{
    short   TipoVehic,
            NumVehic;
    str21   NomPiloto,
            NomCoPiloto;
    str16   MarcaVehic;
    short   TiempoTotal;
};

typedef sEtapasRally tblEtapasRally[MaxReg];
typedef sTiemposTotales tblTiempoTotales[MaxReg];


//------------Prototipos-----------
FILE *Abrir(sEtapasRally);
bool LeerEtapasRally(sEtapasRally &, FILE *);
void Guardar(sEtapasRally, FILE *, short &, tblEtapasRally);
void ProcEtapasRally(sEtapasRally, FILE *, tblEtapasRally, short &);
void Cantidades(tblEtapasRally, short);
void ListadoCorredores(tblEtapasRally, short);
void IntCmbEtapas(sEtapasRally &, sEtapasRally &);
void OrdxBurTipoVehicEtapas(tblEtapasRally, short);
void OrdxBurEtapa(tblEtapasRally, short);
void ListadoLargada(tblEtapasRally, short);
short BusSecVec(tblTiempoTotales, sTiemposTotales, short);
void IntCmbTiempos(sTiemposTotales &,sTiemposTotales &);
void OrdxBurTiempoTotal(tblTiempoTotales, short);
void ListadoPuestosFinal(tblEtapasRally, sTiemposTotales, short, tblTiempoTotales, short &);
void OrdxBurTipoVehicTiempos(tblTiempoTotales, short);
sTiemposTotales BusSecMinTmpxTipoV(tblTiempoTotales, short, short);
void ListadoGanadoresxTipoVehic(tblTiempoTotales, short);
void Cerrar(FILE *);
//----------Fin-Prototipos-----------

main()
{
    short   TotalReg = 0,
            CantVehic = 0;
    sEtapasRally rEtapasRally;
    tblEtapasRally tblEtapas;
    sTiemposTotales rTiemposTotales;
    tblTiempoTotales tblTiempos;
    FILE *ArchivoTxt;

    ArchivoTxt = Abrir(rEtapasRally);
    ProcEtapasRally(rEtapasRally, ArchivoTxt, tblEtapas, TotalReg);
    ListadoCorredores(tblEtapas, TotalReg);
    ListadoLargada(tblEtapas, TotalReg);
    ListadoPuestosFinal(tblEtapas, rTiemposTotales, TotalReg, tblTiempos, CantVehic);
    ListadoGanadoresxTipoVehic(tblTiempos, CantVehic);
    Cerrar(ArchivoTxt);

    return 0;
}

FILE *Abrir(sEtapasRally rRally) // Abrir archivo txt
{
    FILE *f;
	f = fopen("EtapasRally.txt", "r");

    return f;
}

bool LeerEtapasRally(sEtapasRally &rLecRally, FILE *txt) // Lectura de registros con fscanf
{
    char car;

    fscanf(txt,"%d%d%d%c",&rLecRally.Etapa,
                        &rLecRally.TipoVehic,
                        &rLecRally.NumVehic,
                        &car);

    if (!feof(txt))
    {
        fgets(rLecRally.NomPiloto,22,txt);
        fgets(rLecRally.NomCoPiloto,22,txt);
        fgets(rLecRally.MarcaVehic,17,txt);
        fscanf(txt,"%d",&rLecRally.TiempoMins);

        return true;
    }
    return false;
}

void Guardar(sEtapasRally rLecRally, FILE *txt, short &n, tblEtapasRally vrRally) // Llenar array con registros
{
    n++;

    vrRally[n] = rLecRally;
}

void ProcEtapasRally(sEtapasRally rRally, FILE *f, tblEtapasRally vrEtapas, short &ContReg)
{
    while (LeerEtapasRally(rRally,f))
    {
        Guardar(rRally,f,ContReg,vrEtapas);
    }
}

void Cantidades(tblEtapasRally vrRally, short cReg) // Mostrar cantidades solicitadas
{
    short contAbandonos = 0;

    cout << "Registros Leídos: " << cReg << endl; //Cantidad de Registros Leídos

    for (short i=1;i<cReg;i++)
    {
        if (vrRally[i].Etapa == 5 && vrRally[i].TiempoMins == 999)
            contAbandonos++;
    }
    cout << "Vehículos que Abandonaron: " << contAbandonos << endl; //Cantidad de Vehículos que abandonaron
}

void ListadoCorredores(tblEtapasRally vrEtapas, short CantReg) // Ejecución freopen para escritura externa - Listado del array con los campos solicitados
{
    freopen("ListadoRally.txt","w",stdout);

    Cantidades(vrEtapas, CantReg);

    cout << "             Listado de Corredores de Etapas y Tipos de Vehículos" << endl;
    cout << "Etp. TpVehic. Nombre del Piloto    Nombre del CoPiloto  Marca Vehic. Tiempo mins." << endl;

    for (short i=1;i<=CantReg;i++)
    {
        cout.setf(ios::right);
        cout << setw(2) << vrEtapas[i].Etapa;
        cout << setw(7) << vrEtapas[i].TipoVehic;
        cout << setw(26) << vrEtapas[i].NomPiloto;
        cout << vrEtapas[i].NomCoPiloto;
        cout << vrEtapas[i].MarcaVehic;
        cout << setw(4) << vrEtapas[i].TiempoMins << endl;
    }
}

void IntCmbEtapas(sEtapasRally &a,sEtapasRally &b) // Intercambiar elementos de un array
{
    sEtapasRally aux = a;

    a = b;
    b = aux;
}

void OrdxBurTipoVehicEtapas(tblEtapasRally vrRally,short card) // Ordenar por Tipo de Vehículo en forma ascendente
{
    bool ordenado;
    short k = 0;

    do
    {
        ordenado = true;
        k++;

        for (short i=1;i<=card-k;i++)
        {
            if (vrRally[i].TipoVehic > vrRally[i + 1].TipoVehic)
            {
                ordenado = false;
                IntCmbEtapas(vrRally[i],vrRally[i + 1]);
            }
        }
    }
    while (!ordenado);
}

void OrdxBurEtapa(tblEtapasRally vrRally,short card) // Ordenar por Etapa en forma ascendente
{
    bool ordenado;
    short k = 0;

    do
    {
        ordenado = true;
        k++;

        for (short i=1;i<=card-k;i++)
        {
            if (vrRally[i].Etapa > vrRally[i + 1].Etapa)
            {
                ordenado = false;
                IntCmbEtapas(vrRally[i],vrRally[i + 1]);
            }
        }
    }
    while (!ordenado);
}

void ListadoLargada(tblEtapasRally vrEtapas, short CantReg) // Ejecución de ordenamientos - Listado del array con los campos solicitados
{
    OrdxBurTipoVehicEtapas(vrEtapas, CantReg);
    OrdxBurEtapa(vrEtapas, CantReg);

    cout << endl;
    cout << "                       Listado de Largada" << endl;

    for (short i=1;i<=CantReg;i++)
    {
        if (vrEtapas[i].Etapa != vrEtapas[i-1].Etapa)
        {
            cout << endl;
            cout << "Nro. de Etapa: " << vrEtapas[i].Etapa << endl;
        }
        if (vrEtapas[i].TipoVehic != vrEtapas[i-1].TipoVehic)
        {
            cout << "  Tipo de Vehiculo: " << vrEtapas[i].TipoVehic << endl;
            cout << "    Nro. Vehic.   Nom. Piloto          Nom. CoPiloto        Marca Vehic." << endl;
        }
            cout << "        " << setw(2) << vrEtapas[i].NumVehic;
            cout << setw(29) << vrEtapas[i].NomPiloto;
            cout << vrEtapas[i].NomCoPiloto;
            cout << vrEtapas[i].MarcaVehic << endl;
    }
    cout << endl;
}

short BusSecVec(tblTiempoTotales vrTiemp, sTiemposTotales rTiemp, short card) // Ejecución de contador necesario para la busqueda secuencial
{
    short i = 1;

    while (i <= card && vrTiemp[i].NumVehic != rTiemp.NumVehic)
    {
        i++;
    }
    return i;
}

void IntCmbTiempos(sTiemposTotales &a,sTiemposTotales &b) // Intercambiar elementos de un array
{
    sTiemposTotales aux = a;

    a = b;
    b = aux;
}

void OrdxBurTiempoTotal(tblTiempoTotales vrTiemp,short card) // Ordenar por Tiempo Total en forma ascendente
{
    bool ordenado;
    short k = 0;

    do
    {
        ordenado = true;
        k++;

        for (short i=1;i<=card-k;i++)
        {
            if (vrTiemp[i].TiempoTotal > vrTiemp[i + 1].TiempoTotal)
            {
                ordenado = false;
                IntCmbTiempos(vrTiemp[i],vrTiemp[i + 1]);
            }
        }
    }
    while (!ordenado);
}

void ListadoPuestosFinal(tblEtapasRally vrEtapas, sTiemposTotales rTiempos, short CantReg, tblTiempoTotales vrTiempos, short &cVehic) // Llenar array luego de leer registros para nuevo tabla
{
    short posVehic;

    for (short i=1;i<=CantReg;i++)
    {
        rTiempos.TipoVehic = vrEtapas[i].TipoVehic;
        rTiempos.NumVehic = vrEtapas[i].NumVehic;
        strcpy(rTiempos.NomPiloto, vrEtapas[i].NomPiloto);
        strcpy(rTiempos.NomCoPiloto, vrEtapas[i].NomCoPiloto);
        strcpy(rTiempos.MarcaVehic, vrEtapas[i].MarcaVehic);
        rTiempos.TiempoTotal = vrEtapas[i].TiempoMins;

        posVehic = BusSecVec(vrTiempos, rTiempos, cVehic);

        if (posVehic > cVehic)
        {
            cVehic++;
            vrTiempos[posVehic].TipoVehic = rTiempos.TipoVehic;
            strcpy(vrTiempos[posVehic].NomPiloto, rTiempos.NomPiloto);
            strcpy(vrTiempos[posVehic].NomCoPiloto, rTiempos.NomCoPiloto);
            strcpy(vrTiempos[posVehic].MarcaVehic, rTiempos.MarcaVehic);
            vrTiempos[posVehic].NumVehic = rTiempos.NumVehic;
            vrTiempos[posVehic].TiempoTotal = 0;
        }
            vrTiempos[posVehic].TiempoTotal += rTiempos.TiempoTotal;
    }

    OrdxBurTiempoTotal(vrTiempos, cVehic);

    cout << "                             Listado de Puestos Finales Carrera Rally" << endl;
    cout << "Puesto  T. Vehic.  Nro. Vehic.  Nom. Piloto          Nom. CoPiloto        Marca Vehic.   Tiempo Total" << endl;

    for (short i=1;i<=cVehic;i++)
    {
        cout << setw(4) << i << "      ";
        cout << setw(4) << vrTiempos[i].TipoVehic << "      ";
        cout << setw(5) << vrTiempos[i].NumVehic;
        cout << setw(28) << vrTiempos[i].NomPiloto;
        cout << vrTiempos[i].NomCoPiloto;
        cout << vrTiempos[i].MarcaVehic;
        cout << setw(6) << vrTiempos[i].TiempoTotal << endl;
    }
}

void OrdxBurTipoVehicTiempos(tblTiempoTotales vrTiemp,short card)  // Ordenar por Tiempo Total en forma ascendente
{
    bool ordenado;
    short k = 0;

    do
    {
        ordenado = true;
        k++;

        for (short i=1;i<=card-k;i++)
        {
            if (vrTiemp[i].TipoVehic > vrTiemp[i + 1].TipoVehic)
            {
                ordenado = false;
                IntCmbTiempos(vrTiemp[i],vrTiemp[i + 1]);
            }
        }
    }
    while (!ordenado);
}

sTiemposTotales BusSecMinTmpxTipoV(tblTiempoTotales vrTiemp, short TpoVehic, short card) // Completar registros para tabla
{
    sTiemposTotales c;

    for (short i=1;i<=card;i++)
    {
        c.TipoVehic = vrTiemp[i].TipoVehic;
        c.NumVehic = vrTiemp[i].NumVehic;
        strcpy(c.NomPiloto, vrTiemp[i].NomPiloto);
        c.TiempoTotal = vrTiemp[i].TiempoTotal;

        if (c.TipoVehic == TpoVehic)
            return c;
    }
}

void ListadoGanadoresxTipoVehic(tblTiempoTotales vrTiempos, short cVehic) // Ordenamiento - Listado del array con los campos solicitados - freopen para volver a la consola
{
    cout << endl;
    cout << "       Listados de Ganadores por tipos de Vehiculos" << endl;
    cout << "Tipo Vehic.  Nro. Vehic.  Nom. Piloto          Tiempo Total" << endl;

    OrdxBurTipoVehicTiempos(vrTiempos, cVehic);
    sTiemposTotales c;

    for (short i=1;i<=MaxTipos;i++)
    {
        c = BusSecMinTmpxTipoV(vrTiempos, i, cVehic);

        cout << setw(6) << c.TipoVehic << "      ";
        cout << setw(7) << c.NumVehic;
        cout << setw(28) << c.NomPiloto;
        cout << setw(8) << c.TiempoTotal << endl;
    }

    freopen("CON","w",stdout);
}

void Cerrar(FILE *f) // Cerrar archivo
{
    fclose(f);
}

