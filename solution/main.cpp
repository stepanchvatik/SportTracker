#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
///@author Stepan Chvatik
///@date 17.11.2017
///@mainpage CHV0037 Activity Log

using namespace std;

ifstream ifs("..\\vstupnidata\\data.csv");
ofstream ofs("..\\vstupnidata\\data.csv",ios_base::app);
string date;
string type;
double route;
double time;
double calories;
double speed;

//Struct
typedef struct{
    unsigned int sID;
    string sdate;
    string stype;
    string sroute;
    string stime;
    string scalories;
    string sspeed;
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
    while(date[0]-'0' > 3 || date[2]-'0'>1)
    {
        cout << "Zadejte platne datum!" << endl;
        cin >> date;
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
        cout<< "Zadejte platnoy cas!"<<endl;
        cin >> time;
    }
    calories = countCalories(route,time);
    speed = route / (time/60);
    ofs<<generateId() << "," << date << "," << type << "," << route << "," << time << "," << calories << "," << speed<< endl;
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
                case 6: {activity[poradi].sspeed += radek[i]; break;}
           }
        }
        activity[poradi].sID = atoi(ID.c_str());
        poradi++;
    }
}

void generateHtml(Activities activity[])
{
    ofstream html("..\\vystupnidata\\vystup.html");
    string troll = "";
    string replacetroll="<script type=\"text/javascript\">var topik=50;function easterEgg(){document.getElementById('EE').style.top=topik+'px';topik+=56;if(topik==1000){clearInterval(myFunction);}}var myFunction = setInterval(easterEgg,2000);</script><div id=\"EE\" style=\"background-color:white;width:100%;height:100%;position:fixed;top:50px;\"></div>"
    html<<"<html><head><title>SportTracker</title><style>*{cursor:wait;}td{border:2px solid black; text-align:center;transition: ease-in-out 0.3s;} table{margin-bottom:30px;width:40%;margin-left:30%;border:2px solid black;} tr:hover td{background-color:lightgrey;font-weight:bold;}</style></head><body>"<< troll << "<h1 style=\"font-size:50px;text-align:center\">SportTracker</h1>";

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
        bool goout = true;
        for(int j=0;j<generateId();j++)
        {
            if(activity[j].sdate[3]-'0'==i&&goout)
            {
                if(activity[j].stype==type)
                {
                    html<<"<table cellspacing=\"0\"><tr><td colspan=\"100%\" style=\"background-color:yellow;\">" <<i<<". mesic</td></tr><tr><td>Den</td><td>Vzdalenost</td><td>Cas</td><td>Kalore</td><td>Rychlost</td></tr>";
                    goout=false;
                }
            }
        }
        for(int j=0;j<generateId();j++)
        {
            if(activity[j].sdate[3]-'0'==i)
            {
                if(activity[j].stype==type)
                html << "<tr><td>"<< activity[j].sdate[0]<<activity[j].sdate[1] << ".</td><td>" << activity[j].sroute<<"km"<< "</td><td>"<<activity[j].stime<<" minut</td><td>"<<activity[j].scalories<<"kcal</td><td>"<<activity[j].sspeed <<  "km/h</td></tr>";
            }
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
    for(int j=0;j<generateId();j++)
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
    html<<"<h1 style=\"text-align:center;\">Rocni tabulka serazena podle cetnosti</h1><table cellspacing=\"0\"><tr><td colspan=\"100%\" style=\"background-color:yellow;\">ROCNI PREHLED</td></tr><tr><td>Typ</td><td>Den</td><td>Vzdalenost</td><td>Cas</td><td>Kalore</td><td>Rychlost</td></tr>";
    for(int i =0;i<4;i++)
    {
         for(int j=0;j<generateId();j++)
        {

                if(activity[j].stype==newnames[i])
                html << "<tr><td>"<<activity[j].stype<<"</td><td>"<< activity[j].sdate[0]<<activity[j].sdate[1] << "."<<activity[j].sdate[0]<<activity[j].sdate[1]<<".</td><td>" << activity[j].sroute<<"km"<< "</td><td>"<<activity[j].stime<<" minut</td><td>"<<activity[j].scalories<<"kcal</td><td>"<<activity[j].sspeed <<  "km/h</td></tr>";

        }
    }
    html<<"</table>";

    cout<<"Bylo vygenerovano HTML"<<endl;
    html<<"</body></html>";
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
    int fills = generateId();
    fills--;
    Activities activity[fills];
    fillActivities(activity);
    generateId();
    cout << "Vitejte v aplikaci pro zapis sportovnich aktivit" << endl;
    menu(activity);
    return 0;
}
