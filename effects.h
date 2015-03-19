#include <iostream>
#include <fstream>
#include <vector>
#include <QDebug>
#include <QString>

/*
* projet  : Projet de session S2
* fichier : effects.cpp
* auteur  : Eric Matte
* date    : 17-03-2015
*/

#include <string>
#include <time.h>
#include <math.h>

using namespace std;

class RGBColor
{
public:
	RGBColor();
	RGBColor(const double&, const double&, const double&);
    RGBColor(const char* hexColor);
	void setNewColor(const double&, const double&, const double&);
	void calculateColor(int operation,RGBColor &color1, RGBColor &color2,int argument);
	void calculateColor(vector<double> RedOpp, vector<double> GreenOpp, vector<double> BlueOpp, int argument);
	int getR() const;
	int getG() const;
	int getB() const;

private:
	double r;
	double g;
	double b;
};

class EffectsGenerator
{
public:
	EffectsGenerator(vector<RGBColor> const& colorsGrid);
	void gradientTransition(int transitionSize, string &os);
	void gradientSigmoidTransition(int transitionSize, string &os);
	vector<double> SigmoidCalculator(double Xi, double Xf, double Yi, double Yf);
	void strobeTransition(int transitionTime, bool random, string &os);
private:
	vector<RGBColor> colors; //tableau dynamique de couleurs

};















































