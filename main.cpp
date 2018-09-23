#include <iostream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <iterator>
#include <string>

using namespace std;

typedef struct coordenada{
    int row;
    int column;
    int value;
};

int array_size=9;
int subarray_size=array_size/3;
int max_value=9;
bool error=false;

vector<string> split(const string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

void load_cordinates(vector<coordenada>& coordenadas, string argumento)
{
    vector<string> vector_arg_aux = split(argumento,']');
    for(int i=0;i<vector_arg_aux.size();i++)
    {
        vector_arg_aux.at(i).erase(0,1);
        vector<string> vector_aux = split(vector_arg_aux.at(i),';');
        stringstream value1(vector_aux.at(0)), value2(vector_aux.at(1)), value3(vector_aux.at(2));
        coordenada coordenada_aux;
        value1>>coordenada_aux.row;value2>>coordenada_aux.column;value3>>coordenada_aux.value;
        if((coordenada_aux.row<array_size && coordenada_aux.row>=0) || (coordenada_aux.column<array_size && coordenada_aux.column>=0) || (coordenada_aux.value<=max_value && coordenada_aux.value>=0) )
            coordenadas.push_back(coordenada_aux);
        else
        {
            cout<<"Se han ingresado valores incorrectos\nLos valores maximos son:\n\tTamanio Matriz: "<<array_size<<"\n\tValor Maximo: "<<max_value<<endl;
            error=true;
            break;
        }
    }

}

void load_zeroz_array(vector< vector<int> >& int_array, int sizes)
{
    vector<int> aux(sizes);
    for(int i = 0 ; i < sizes ; i++) int_array.push_back(aux);
}

void load_array(vector< vector<int> >& int_array, string argumento)
{
    vector<coordenada> coordenadas;
    load_cordinates(coordenadas, argumento);
    for(int i = 0 ; i < coordenadas.size() ; i++) int_array[coordenadas.at(i).row][coordenadas.at(i).column]=coordenadas.at(i).value;
}
void print_array(vector< vector<int> > int_array)
{
    for(int i = 0 ; i < array_size ; i++)
        for(int j = 0 ; j < array_size ; j++)
            if(j<array_size-1)
                cout<<int_array[i][j]<<" - ";
            else
                cout<<int_array[i][j]<<endl;
    cout<<endl;
}

bool Revisar(int pivot, int value, vector< vector<int> > int_array, bool is_row)
{
    bool result=false;
    for(int i = 0 ; i < int_array.size() ; i++)
    {
        if(is_row)
            result=(int_array[pivot][i]==value);
        else
            result=(int_array[i][pivot]==value);
        if(result)
            break;
    }
    return result;
}

bool RevisarFila(int row, int value, vector< vector<int> > int_array)
{
    return Revisar(row, value, int_array, true);
}

bool RevisarColumna(int column, int value, vector< vector<int> > int_array)
{
    return Revisar(column, value, int_array, false);
}

bool RevisarSubMatriz(int subMatriz, int value, vector< vector<int> > int_array)
{
    vector< vector<int> > int_subarray;
    bool result=false;
    load_zeroz_array(int_subarray,subarray_size);
    for(int i = 0, i2=(subMatriz<4)?0:((subMatriz<7)?3:6) ; i < subarray_size ; i++,i2++)
            for(int j = 0, j2=(subMatriz%3)?(((subMatriz+1)%3)?0:3):6; j < subarray_size ; j++,j2++)
                int_subarray[i][j]=int_array[i2][j2];
    for(int i = 0 ; i < subarray_size ; i++)
        if(RevisarColumna(i, value, int_subarray))
        {
            result=true;
            break;
        }
    return result;
}

void mensaje(bool value, string message)
{
    if(value)
        cout << endl << "El valor SI se encuentra en la "<<message<<" indicada" << endl << endl;
    else
        cout << endl << "El valor NO se encuentra en la "<<message<<" indicada" << endl << endl;
}

void menu(int& option)
{
    cout<<"Ingrese opcion: \n\t1.- Mostrar Matriz\n\t2.- Revisar en Fila\n\t3.- Revisar en Columna\n\t4.- Revisar en SubMatriz\n\t0.- Salir\n";
    cin>>option;
    cout<<endl;
}

void datos(int& value1, int& value2, string message)
{
    bool correct;
    do
    {
        cout<<"Ingrese "<<message<<": ";cin>>value1;
        cout<<"Ingrese valor: ";cin>>value2;
        correct=(value1<=max_value && value2<=max_value && value1>=0 && value2>=0);
        if(!correct)
            cout<<"\nLos valores ingresados no pueden ser negativos o superar el valor: "<<max_value<<"\n\n";
    }while(!correct);
}

void opciones(vector< vector<int> > int_array)
{
    int row, value, column, option=4;
    string fila = "fila", columna = "columna",subMatriz="subMatriz";
    if(!error)
    {
        do
        {
            menu(option);
            switch(option)
            {
            case 0:
                break;
            case 1:
                print_array(int_array);
                break;
            case 2:
                datos(row, value, fila);
                mensaje(RevisarFila(row, value, int_array), fila);
                break;
            case 3:
                datos(column, value, columna);
                mensaje(RevisarColumna(column, value, int_array), columna);
                break;
            case 4:
                datos(column, value, subMatriz);
                mensaje(RevisarSubMatriz(column, value, int_array), subMatriz);
                break;
            default:
                cout << "\nOpcion invalida\n" << endl;
                break;
            }
        }while(option);
    }
}

int main(int argc, char *argv[])
{
    vector< vector<int> > int_array;
    load_zeroz_array(int_array,array_size);
    if(argc == 2)
        load_array(int_array, string(argv[1]));
    else
        if(argc-1)
            {
                cout<<"Se han ingresado demasiados argumentos\n";
                return 0;
            }
    opciones(int_array);
    return 0;
}
