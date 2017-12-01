#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

///@author Stepan Chvatik
///@date 17.11.2017
///@mainpage CHV0037 Activity Log

using namespace std;

ifstream ifs("..\\data\\data.csv");
ofstream ofs("..\\data\\data.csv",ios_base::app);
string date;
string type;
double route;
double time;
double calories;

//Struct
typedef struct{
    unsigned int sID;
    string sdate;
    string stype;
    double sroute;
    double stime;
    double scalories;
}  Activities;

void menu(Activities activity[]);
int generateId();
double countCalories(double dist, double time);





//Function for user input
void writeActivity(Activities activity[])
{

    cout<<"Bylo vlozeno ID aktivity"<<endl;

    cout<<"Zadejte datum ve formatu DDMMYYYY: ";
    cin >> date;
    cout<<"Zadejte typ aktivity (chuze, beh, kolo, inline): ";
    cin>>type;
    while(type!="beh"&&type!="kolo"&&type!="chuze"&&type!="inline")
    {
        cout<<"Zadejte platnou aktivitu!" << endl;
        cin >> type;
    }
    cout << "Zadejte vzdalenost v km: ";
    cin >> route;
    cout << "Zadejte cas v minutach: ";
    cin >>time;
    calories = countCalories(route,time);
    ofs<<generateId() << "," << date << "," << type << "," << route << "," << time << "," << calories << endl;
    ofs.close();

    menu(activity);
}

//ID for each row in CSV file
int generateId()
{

    int id = 0;
    string a;
    while(getline(ifs, a))
          {
              id++;
          }
    ifs.clear();
    ifs.seekg(0,std::ios::beg);
    return id+1;

}

//Counting Calories
double countCalories(double dist, double time)
{
    double caloriesCount = dist/(time/60)*time * 0.2;
    return caloriesCount;
}

//Filling activities struct
void fillActivities(Activities activity[])
{
    string radek;
    int poradi=0;
    while (getline(ifs, radek)){
        string ID = "";
        int pozice = 0;
        for(unsigned int i=0;i<radek.length();i++)
        {
           if(radek[i]==',')
           {
               pozice++;
               continue;
           }
           switch(pozice)
           {
                case 0: {ID += radek[i]; break;}
                case 1: {activity[poradi].sdate += radek[i]; break;}
                case 2: {activity[poradi].stype += radek[i]; break;}
                case 3: {activity[poradi].sroute += radek[i]; break;}
                case 4: {activity[poradi].stime += radek[i]; break;}
                case 5: {activity[poradi].scalories += radek[i]; break;}
           }
        }
        activity[poradi].sID = atoi(ID.c_str());
        poradi++;
    }
}

void generateHtml(Activities activity[])
{
    ofstream html("..\\html\\vystup.html");
    cout<<"Bylo vygenerovano HTML"<<endl;
    html << "DATA FROM Activity struct";
    html.flush();
    menu(activity);
}

//MENU
void menu(Activities activity[])
{
    cout << "Vyberte si akci:" <<endl;
    cout << "1 - zapis aktivity\n2 - vygenerovani HTML\n3 - konec programu" << endl;
    int option;
    cin >>option;
    while(option!=1&&option!=2&&option!=3)
    {
        cout<<"Zvolte platnou volbu!" << endl;
        cin >> option;
    }
    switch(option)
    {
    case 1:
        writeActivity(activity);
        break;
    case 2:
        generateHtml(activity);
        break;
    case 3:
        exit(0);
    }
}


int main()
{
    Activities activity[generateId()-1];
    cout << generateId() <<endl;
    fillActivities(activity);
    cout << activity[0].stype;
    cout << "Vitejte v aplikaci pro zapis sportovnich aktivit" << endl;
    menu(activity);
    return 0;
}
