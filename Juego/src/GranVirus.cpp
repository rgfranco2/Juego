#include "GranVirus.h"

GranVirus::GranVirus(float altura, float anchura, float x, float y, float vx, float vy) :Enemigo(altura, anchura, x, y, vx, vy)
{
	setTipo(GRANVIRUS);
	sprite = new SpriteSequence("imagenes/virus.png", 1, 1, 300, true, x, y, anchura, altura);
	sprite->setState(0, false);
	vida = 3;
	puntuacion = 5;
}

void GranVirus::dibuja() {
	Enemigo::dibuja();
	disparos.dibuja();

	if (vida == 3)BarradeVida->setState(0, false);
	else if (vida == 2)BarradeVida->setState(3, false);
	else if (vida == 1)BarradeVida->setState(6, false);
	BarradeVida->setPos(posicion.x, posicion.y + altura / 2);
	BarradeVida->draw();
}

void GranVirus::division(ListaEnemigos& enemigos)
{
	enemigos.agregar(new Minivirus(6, 6, posicion.x - 5, posicion.y - 3, -1, 0));
	enemigos.agregar(new Minivirus(6, 6, posicion.x + 5, posicion.y - 3, -1, 0));
}
