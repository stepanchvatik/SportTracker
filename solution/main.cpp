#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
///@brief Hlavni soubor programu
///@author Stepan Chvatik
///@date 17.11.2017
///@mainpage CHV0037 Activity Log
///@file main.cpp

using namespace std;

ifstream ifs("..\\vstupnidata\\data.csv");
ofstream ofs("..\\vstupnidata\\data.csv",ios_base::app);
string date;
string type;
double route;
double time;
double calories;
double speed;
int *mesicniSouhrn = (int*)malloc(sizeof(int));
///@struct Activities
///@brief Struktura obsahujici vsechny aktivity
///@param Activities.sID            Unikatni ID kazde aktivity
///@param Activities.sdate          Datum konani aktivity DDMMYYYY
///@param Activities.stype          Typ aktivity
///@param Activities.sroute         Vzdalenost
///@param Activities.stime          Cas aktivity
///@param Activities.scalories      Vypoctene kalorie
///@param Activities.sspeed         Dopoctena rychlost

typedef struct{
    unsigned int sID;
    string sdate;
    string stype;
    string sroute;
    string stime;
    string scalories;
    string sspeed;
}  Activities;


///@brief Funkce pro volani menu
///@param activity      Predavani pole ze struktury
void menu(Activities activity[]);

///@brief Funkce pro zjisteni poctu radku v CSV (generovani nasledujiciho ID)
int generateId();

///@brief Funkce pro vypocet kalorii
///@param dist      Potrebna vzdalenost aktivity
///@param cas      Potrebny cas aktivity
double countCalories(double dist, double time);




///@brief Funkce pro zapsani aktivity
void writeActivity(Activities activity[])
{

    cout<<"Bylo vlozeno ID aktivity"<<endl;

    cout<<"Zadejte datum ve formatu DDMMYYYY: ";
    cin >> date;
    int day;
    int month;
    int year;
    day = atoi(date.substr(0,2).c_str());
    month = atoi(date.substr(2,2).c_str());
    year = atoi(date.substr(4,4).c_str());
    while(day>31||month>12||year>2017)
    {
        cout << "Zadejte platne datum!" << endl;
        cin >> date;
        day = atoi(date.substr(0,2).c_str());
        month = atoi(date.substr(2,2).c_str());
        year = atoi(date.substr(4,4).c_str());
    }
    cout<<"Zadejte typ aktivity (chuze, beh, kolo, inline): ";
    cin>>type;
    while(type!="beh"&&type!="kolo"&&type!="chuze"&&type!="inline")
    {
        cout<<"Zadejte platnou aktivitu!" << endl;
        cin >> type;
    }

    cout << "Zadejte vzdalenost v km: ";
    cin >> route;
    while(cin.fail())
    {
        cout<< "Zadejte platnou vzdalenost!"<<endl;
        cin >> route;
    }

    cout << "Zadejte cas v minutach: ";
    cin >>time;
    while(cin.fail())
    {
        cout<< "Zadejte platny cas!"<<endl;
        cin >> time;
    }
    calories = countCalories(route,time);
    speed = route / (time/60);
    ofs<<generateId() << "," << date << "," << type << "," << route << "," << time << "," << calories << "," << speed<< endl;
    ofs.close();

    menu(activity);
}

///@brief Funkce generovani ID viz vyse
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

///@brief Realizace funkce vypoctu kalorii
double countCalories(double dist, double time)
{
    double caloriesCount = dist/(time/60)*time * 0.2;
    return caloriesCount;
}

///@brief Funkce pro naplneni pole struktury
///@param activity      Predavani pole
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
                case 6: {activity[poradi].sspeed += radek[i]; break;}
           }
        }
        activity[poradi].sID = atoi(ID.c_str());
        poradi++;
    }
}



///@brief Funkce pro generovani HTML
void generateHtml(Activities activity[])
{
    ofstream html("..\\vystupnidata\\vystup.html");
    html<<"<html>"<<endl;
    html<<"<head>"<<endl;
    html<<"<title>SportTracker</title>"<<endl;
    html<<"<style>"<<endl;
    html<<"*{}"<<endl;
    html<<"body{background: linear-gradient(to right, rgba(95, 211, 255, 0.68), rgb(158, 255, 175));}"<<endl;
    html<<"td{border:2px solid black; text-align:center;transition: ease-in-out 0.3s;background-color:white;}"<<endl;
    html<<"table{margin-bottom:30px;width:40%;margin-left:30%;border:2px solid black;}"<<endl;
    html<<"tr:hover td{background-color:lightgrey;font-weight:bold;}"<<endl;
    html<<"</style>"<<endl;
    html<<"</head>"<<endl;
    html<<"<body>"<<endl;
    html<<"<h1 style=\"font-size:50px;text-align:center\">SportTracker</h1>";


    string subdate;
    int month;
    int day;
    //Vyber aktivity pro mesicni souhrn a prumerna rychlost

    cout<<"Zadejte typ aktivity (chuze, beh, kolo, inline): ";
    cin>>type;
    while(type!="beh"&&type!="kolo"&&type!="chuze"&&type!="inline")
    {
        cout<<"Zadejte platnou aktivitu!" << endl;
        cin >> type;
    }
    html<<"<h1 style=\"text-align:center;\">Mesicni souhrn aktivity "<<type<<"</h1>";



    for(int i=1;i<13;i++)
    {
        int counter=0;
        bool heading = true;
        for(int j=0;j<generateId()-1;j++)
        {
            subdate=activity[j].sdate;
            subdate = subdate.substr(2,2);
            month = atoi(subdate.c_str());
            if(month==i)
            {
                if(activity[j].stype==type)
                {
                    if(heading)
                    html<<"<table cellspacing=\"0\"><tr><td colspan=\"100%\" style=\"background-color:yellow;\">" <<i<<". mesic</td></tr><tr><td>Den</td><td>Vzdalenost</td><td>Cas</td><td>Kalorie</td><td>Rychlost</td></tr>";
                    mesicniSouhrn[counter]=j;
                    counter++;
                    heading=false;
                }
            }
        }
        for(int j=0;j<counter;j++)
        {
            subdate=activity[mesicniSouhrn[j]].sdate;
            subdate = subdate.substr(2,2);
            month = atoi(subdate.c_str());
            subdate=activity[mesicniSouhrn[j]].sdate;
            subdate = subdate.substr(0,2);
            day = atoi(subdate.c_str());
            html << "<tr><td>"<< day << ".</td><td>" << activity[mesicniSouhrn[j]].sroute<<"km"<< "</td><td>"<<activity[mesicniSouhrn[j]].stime<<" minut</td><td>"<<activity[mesicniSouhrn[j]].scalories<<"kcal</td><td>"<<activity[mesicniSouhrn[j]].sspeed <<  "km/h</td></tr>";

        }

        html<<"</table>";
    }


    int pocet[4];
    string names[4] = {"inline","beh","chuze","kolo"};
    string newnames[4];
    pocet[0]=0; //inline
    pocet[1]=0; //beh
    pocet[2]=0; //chuze
    pocet[3]=0; //kolo
    for(int j=0;j<generateId()-1;j++)
    {
            if(activity[j].stype=="inline")
                pocet[0]++;
            else if(activity[j].stype=="beh")
                pocet[1]++;
            else if(activity[j].stype=="chuze")
                pocet[2]++;
            else if(activity[j].stype=="kolo")
                pocet[3]++;
    }

    for(int i = 0;i<4;i++)
    {
        int tmp = pocet[0];
        int index=0;
        for(int j = 1;j<4;j++)
        {
            if(tmp<pocet[j])
            {
                tmp=pocet[j];
                index=j;
            }

        }
        pocet[index]=0;

        newnames[i] = names[index];
    }
    html<<"<h1 style=\"text-align:center;\">Rocni tabulka serazena podle cetnosti</h1><table cellspacing=\"0\"><tr><td colspan=\"100%\" style=\"background-color:yellow;\">ROCNI PREHLED</td></tr><tr><td>Typ</td><td>Den</td><td>Vzdalenost</td><td>Cas</td><td>Kalorie</td><td>Rychlost</td></tr>";
    for(int i =0;i<4;i++)
    {
         for(int j=0;j<generateId()-1;j++)
        {

                if(activity[j].stype==newnames[i])
                html << "<tr><td>"<<activity[j].stype<<"</td><td>"<< activity[j].sdate[0]<<activity[j].sdate[1] << "."<<activity[j].sdate[2]<<activity[j].sdate[3]<<".</td><td>" << activity[j].sroute<<"km"<< "</td><td>"<<activity[j].stime<<" minut</td><td>"<<activity[j].scalories<<"kcal</td><td>"<<activity[j].sspeed <<  "km/h</td></tr>";

        }
    }
    html<<"</table>";

    cout<<"Bylo vygenerovano HTML"<<endl;
    html<<"</body></html>";
    html.flush();
    menu(activity);
}

///@brief Funkce pro volani menu
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
    int fills = generateId();
    fills--;
    Activities activity[fills];
    fillActivities(activity);
    generateId();
    cout << "Vitejte v aplikaci pro zapis sportovnich aktivit" << endl;
    menu(activity);
    return 0;
}
