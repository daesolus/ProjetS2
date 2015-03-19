/*
 * projet  : Projet de session S2
 * fichier : effects.cpp
 * auteur  : Eric Matte
 * date    : 17-03-2015
 */

#include "effects.h"
/*
void main()
{
	vector<RGBColor> colors;
	colors.push_back(RGBColor(200,0,0));
	colors.push_back(RGBColor(81,179,11));
	colors.push_back(RGBColor(200, 0, 0));
	colors.push_back(RGBColor(55, 63, 105));
	colors.push_back(RGBColor(0,135,71));

	EffectsGenerator efx(colors);


	ofstream fichier("img.ppm");
	if (fichier.is_open())
	{
		int transitionSize = 500;
		fichier << "P3\n" << (transitionSize*(colors.size()-1)) << " " << transitionSize << "\n255\n";
		string texte;

		efx.gradientSigmoidTransition(transitionSize, texte);
		
		for (int i = 0; i < transitionSize; i++)
			fichier << texte << endl;

		fichier.close();
	}

}
*/


EffectsGenerator::EffectsGenerator(vector<RGBColor> const& colorsGrid) : colors(colorsGrid)
{
	srand (time(NULL));
}



void EffectsGenerator::gradientTransition(int transitionSize, string &os)
{
	RGBColor tempColor, stepColor;

	for (int j = 0; j < colors.size() - 1; j++)
	{
		stepColor.calculateColor(0, colors[j], colors[j + 1], transitionSize - 1);

		for (int i = 0; i < transitionSize; i++)
		{
			tempColor.calculateColor(1, colors[j], stepColor, i);

			//Envoyer les valeurs aux leds, ajouter un delais ou changer le transitionSize
            os += to_string(tempColor.getR()) + " "
				+ to_string(tempColor.getG()) + " "
				+ to_string(tempColor.getB()) + " ";
			//A remplacer par:
			//sendColorToServer(tempColor.getR(),tempColor.getG(),tempColor.getB())
		}
	}

}

vector<double> EffectsGenerator::SigmoidCalculator(double Xi, double Xf, double Yi, double Yf)
{
	vector<double> operations;
	int k = 6;// Facteur de precision
	int offset = 4;
	operations.push_back(((Yi>Yf) ? (Yi + offset) : (Yi - offset))); //new Yi
	operations.push_back(((Yi>Yf) ? (Yf - offset) : (Yf + offset))); //new Yf
	operations.push_back(k / (Xf - Xi)); //a 
	operations.push_back((Xf + Xi) / 2.0); //b

	return operations;
}

void EffectsGenerator::gradientSigmoidTransition(int transitionSize, std::string &os)
{
	RGBColor tempColor;

	for (int j = 0; j < colors.size() - 1; j++)
	{
		vector<double> redOperations = SigmoidCalculator(0, transitionSize, colors[j].getR(), colors[j + 1].getR());
		vector<double> greenOperations = SigmoidCalculator(0, transitionSize, colors[j].getG(), colors[j + 1].getG());
		vector<double> blueOperations = SigmoidCalculator(0, transitionSize, colors[j].getB(), colors[j + 1].getB());

		for (int i = 0; i < transitionSize; i++)
		{
			tempColor.calculateColor(redOperations, greenOperations, blueOperations, i);

			//Envoyer les valeurs aux leds, ajouter un delais ou changer le transitionSize
            os += to_string(tempColor.getR()) + " "
				+ to_string(tempColor.getG()) + " "
				+ to_string(tempColor.getB()) + " ";
			//A remplacer par:
			//sendColorToServer(tempColor.getR(),tempColor.getG(),tempColor.getB())
		}

	}

}


void EffectsGenerator::strobeTransition(int transitionTime, bool random, string &os)
{
	if (!random)
	{
		for (int j = 0; j < colors.size(); j++)
		{
			for (int i = 0; i < transitionTime; i++)
			{
			//Envoyer les valeurs aux leds, ajouter un delais ou changer le transitionSize
			os += to_string(colors[j].getR()) + " "
				+ to_string(colors[j].getG()) + " " 
			    + to_string(colors[j].getB()) + " ";
			//A remplacer par:
			//sendColorToServer(colors[j].getR(),colors[j].getG(),colors[j].getB())
			}
		}
	}
	else
	{
		int currentColor = 0;
		int randNumber = -1;
		int min = transitionTime/4;
		int max = transitionTime;

		for (int i = 0; i < transitionTime*colors.size(); i++)
		{
			if (randNumber<i)
			{
				randNumber = min + (rand() % (int)(max - min + 1));
			}

			for (int j = 0; j < randNumber; j++)
			{
			
				//Envoyer les valeurs aux leds, ajouter un delais ou changer le transitionSize
				os += to_string(colors[currentColor].getR()) + " "
					+ to_string(colors[currentColor].getG()) + " " 
					+ to_string(colors[currentColor].getB()) + " ";
				//A remplacer par:
				//sendColorToServer(colors[currentColor].getR(),colors[currentColor].getG(),colors[currentColor].getB())
				i++;

				if (i >= transitionTime*colors.size())
					break;
			}
			i--;
			currentColor++;
			if (currentColor>=colors.size())
				currentColor=0;
		}
	}
}





////////////////////////////////////////////////
///*****************RGBColor*****************///
////////////////////////////////////////////////

//Calculs necessessaire pour la graduation
void RGBColor::calculateColor(int operation,RGBColor &color1, RGBColor &color2, int argument)
{
	switch (operation)
	{
	case 0:
		setNewColor((color2.r - color1.r) / (argument-1),
					(color2.g - color1.g) / (argument-1),
					(color2.b - color1.b) / (argument-1));
		break;
	case 1:
		setNewColor((color1.r + (color2.r*argument)),
					(color1.g + (color2.g*argument)),
					(color1.b + (color2.b*argument)));
		break;
	default:
		break;
	}
}

//Calculs necessessaire pour la graduation en sigmoid
void RGBColor::calculateColor(vector<double> redOpp, vector<double> greenOpp, vector<double> blueOpp, int argument)
{
	// 0 = Yi
	// 1 = Yf
	// 2 = a
	// 3 = b
	double red = (redOpp[0] - redOpp[1]) / (1 + exp(redOpp[2] * (argument - redOpp[3]))) + redOpp[1];
	double green = (greenOpp[0] - greenOpp[1]) / (1 + exp(greenOpp[2] * (argument - greenOpp[3]))) + greenOpp[1];
	double blue = (blueOpp[0] - blueOpp[1]) / (1 + exp(blueOpp[2] * (argument - blueOpp[3]))) + blueOpp[1];

	setNewColor(((red > 255) ? 255 : ((red < 0) ? 0 : red)),
				((green > 255) ? 255 : ((green < 0) ? 0 : green)),
				((blue > 255) ? 255 : ((blue < 0) ? 0 : blue)));

	/*if (y > 255)
		y = 255;
	if (y < 0)
		y = 0;¸*/
}


RGBColor::RGBColor()
{
	setNewColor(0,0,0);
}
RGBColor::RGBColor(const double& red,const double& green,const double& blue)
{
	setNewColor(red,green,blue);
}

RGBColor::RGBColor(const char* hexColor)
{
    if (strlen(hexColor) == 7) {
        QString str1, str2, str3;
        
        str1.append(hexColor[1]);
        str1.append(hexColor[2]);
        str2.append(hexColor[3]);
        str2.append(hexColor[4]);
        str3.append(hexColor[5]);
        str3.append(hexColor[6]);
        
        str1.toInt();
        str2.toInt();
        str3.toInt();
        
        qDebug() << str1.toInt() << str2.toInt() << str3.toInt();
        
    }else{
        
        qDebug() << "Not the right color lenght, apparently";
    }
}
void RGBColor::setNewColor(const double &red, const double &green, const double &blue)
{
	r = ((red > 255) ? 255 : ((red < 0) ? 0 : red));
	g = ((green > 255) ? 255 : ((green < 0) ? 0 : green));
	b = ((blue > 255) ? 255 : ((blue < 0) ? 0 : blue));
}

int RGBColor::getR() const
{
	return (int)r;
}
int RGBColor::getG() const
{
	return (int)g;
}
int RGBColor::getB() const
{
	return (int)b;
}
