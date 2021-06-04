#include "Mundo.h"
#include "glut.h"
#include <Windows.h>
#include <stdio.h>
#include <string>
#include "ETSIDI.h"


void texto(float x, float y, const char* string);

Mundo::Mundo()
{
	BarradeVida = new SpriteSequence("imagenes/barradevida.png", 5, 1, 100, true, x_ojo-42,46, 15,3,1);
}

void Mundo::dibuja()
{
	x_pto_ojo = personaje.posicion.x;

	x_ojo = personaje.posicion.x;
	if (x_ojo < 42)
	{
		x_ojo = 42;
		x_pto_ojo = 42;
	}

	gluLookAt(x_ojo, y_ojo, z_ojo,   // posicion del ojo
		x_pto_ojo, y_pto_ojo, z_pto_ojo,       // hacia que punto mira  (0,0,0) 
		0.0, 1.0, 0.0);      // definimos hacia arriba (eje Y)  

	//aqui es donde hay que poner el codigo de dibujo

	personaje.dibuja();
	nivel.dibuja();
	bonus.dibuja();
	enemigos.dibuja();

	//barra de vida
	//no s� como ponerla por delante de los suelos

	if (personaje.vida==5)BarradeVida->setState(0, false);
	else if (personaje.vida == 4)BarradeVida->setState(1, false);
	else if (personaje.vida == 3)BarradeVida->setState(2, false);
	else if (personaje.vida == 2)BarradeVida->setState(3, false);
	else if (personaje.vida == 1)BarradeVida->setState(4, false);
	BarradeVida->setPos(x_ojo - 34, 46);
	BarradeVida->draw();



	if (time > 1000)
	{
		tiempo_nivel += 1.0 / 100.0;
		time = 0;
	}

	string str = "Vida: ";
	string vida = to_string(personaje.vida);
	string str2 = "Escudo: ";
	string escudo = to_string(personaje.escudo);
	string str3 = "Tiempo: ";
	string tiempo = to_string(int(tiempo_nivel));
	string str4 = "Velocidad X: ";
	string velx = to_string(float(personaje.getVel().x));
	string str5 = "Velocidad Y: ";
	string vely = to_string(float(personaje.getVel().y));
	string str6 = "Tecla Bloq: ";
	string tecla(1, personaje.tecla_bloq);
	str.append(vida);
	str2.append(escudo);
	str3.append(tiempo);
	str4.append(velx);
	str5.append(vely);
	str6.append(tecla);
	texto(x_ojo - 42, 46, &str[0]);
	texto(x_ojo - 42, 44, &str2[0]);
	texto(x_ojo - 42, 42, &str4[0]);
	texto(x_ojo - 42, 40, &str5[0]);
	texto(x_ojo - 42, 38, &str6[0]);
	texto(x_ojo + 30, 46, &str3[0]);
}

void Mundo::mueve()
{
	static const float t = FREC / 1000.0;

	personaje.mueve(t);
	enemigos.mueve(t);
	bonus.mueve(t);
	/*for (int i = 0; i < tiempo_nivel; i += 1) {
		if(enemigos.lista[i]->tipo==Enemigo::MURCIELAGO)
			reinterpret_cast<Murcielago*>(enemigos.lista[i])->dispara(0, 10.0f, 0);
	}*/
	Interaccion::choque(personaje.disparos, enemigos);
	Interaccion::choque(personaje, bonus);
	Interaccion::choque(personaje.disparos, nivel);
	for (int i = 0; i < enemigos.numero; i++)
	{
		Interaccion::choque(enemigos.lista[i]->disparos, nivel);
		Interaccion::choque(enemigos.lista[i]->disparos, personaje);
	}
	Interaccion::rebote(personaje, nivel);
	Interaccion::atacar(enemigos, personaje);
}

void Mundo::inicializa()
{
	nivel.inicia(bonus, enemigos);
}

void Mundo::tecla(unsigned char key)
{
	switch (key)
	{
	case 'a':
	{
		personaje.setVel(-8, personaje.getVel().y);
		break;
	}
	case 'd':
	{
		personaje.setVel(8, personaje.getVel().y);
		break;
	}
	case 'w':
	{
		if (personaje.tecla_bloq != 'w')
		{
			personaje.setVel(personaje.getVel().x, 16);
			if (personaje.getVel().x > 0.01)
			{
				personaje.setVel(8, personaje.getVel().y);
			}
			else if (personaje.getVel().x < -0.01)
			{
				personaje.setVel(-8, personaje.getVel().y);
			}
		}
		personaje.tecla_bloq = 'w';
		break;
	}
	case 't':
	{
		personaje.posicion.x = 150;
		break;
	}
	}
}

void Mundo::teclaEspecial(unsigned char key)
{

	switch (key)
	{
	case GLUT_KEY_UP:
	{
		personaje.dispara(0, 10.0f, 0);
			//Lo de abajo no lo veo como solucion definitiva porq todos los enemigos de lista
			//de enemigos se pueden convertir a ladrones
			// 
			//Ladron* l = dynamic_cast<Ladron*>(enemigos.lista[i]);
			//if (l) l->dispara(0, 10.0f, 0);
		break;
	}
	case GLUT_KEY_RIGHT:
	{
		personaje.dispara(10.0f, 0, 270);
		break;
	}
	case GLUT_KEY_LEFT:
	{
		personaje.dispara(-10.0f, 0,90);
		break;
	}
	}
}

//Posiblememnte a�adir esto a una clase Texto ?
void texto(float x, float y, const char* string)
{
	glColor3f(0, 255, 0);
	glRasterPos3f(x, y, 0.01);
	int len, i;
	len = strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
	}
}