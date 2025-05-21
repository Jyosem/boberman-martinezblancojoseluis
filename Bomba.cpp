#pragma once
#include "Bomba.h"

CBomba::CBomba(int x, int y) {
	// Calculate the center of the grid cell where the player is
	int cellX = (x / ANCHOIMAGEN);
	int cellY = (y / ALTOIMAGEN);

	// Set bomb position to the center of the cell
	// Centering the bomb graphic in the middle of the cell
	this->x = cellX * ANCHOIMAGEN + (ANCHOIMAGEN / 2) - ((ANCHOIMAGEN * 0.7) / 2);
	this->y = cellY * ALTOIMAGEN + (ALTOIMAGEN / 2) - ((ALTOIMAGEN * 0.7) / 2);

	// Store grid coordinates for explosion calculations
	//this->cellX = cellX;
	//this->cellY = cellY;

	estado = EBomba::puesto;
	tiempoExplosion = 0;
	indiceX = 0;

	// para explosion
	indiceEX = 0;
	indiceEY = 0;
}

EBomba CBomba::getEstado() {
	return estado;
}

void CBomba::animarExplosion() {
	if (indiceEX >= 0 && indiceEX < 3) {
		indiceEX++;
	}
	else {
		estado = EBomba::explotado;
	}
}

bool CBomba::verificarLugar(int jugadorX, int jugadorY, const CanchaArray& cancha) {
	int celda_jugadorX = jugadorX / ANCHOIMAGEN;
	int celda_jugadorY = jugadorY / ALTOIMAGEN;

	if ((cancha[celda_jugadorY][celda_jugadorX]->getTipo() == EElementos::piso) ||
		(cancha[celda_jugadorY][celda_jugadorX]->getTipo() == EElementos::libre)) {
		return true;
	}

	return false;
}

void CBomba::dibujar(Graphics^ g, Bitmap^ bmpBomba, int jugadorX, int jugadorY, const CanchaArray& cancha) {
	if (verificarLugar(jugadorX, jugadorY, cancha)) {
		Rectangle rectangulo = Rectangle(indiceX * ANCHOBOMBA, 0, ANCHOBOMBA, ALTOBOMBA);
		Rectangle zoom = Rectangle(x, y, ANCHOIMAGEN * 0.7, ALTOIMAGEN * 0.7);
		g->DrawImage(bmpBomba, zoom, rectangulo, GraphicsUnit::Pixel);
		if (tiempoExplosion == TIEMPOEXPLOSION) {
			estado = EBomba::explosion;
		}
	}
}

void CBomba::animar() {
	if ((indiceX >= 0) && (indiceX < 2)) {
		indiceX++;
	}
	else {
		tiempoExplosion++;
		indiceX = 0;
	}
}

void CBomba::dibujarExplosion(Graphics^ g, Bitmap^ bmpExplosion, const CanchaArray& cancha) {
	// Usamos el tamano estandar para mantener consistencia
	const int explosionWidth = ANCHOIMAGEN - 10;
	const int explosionHeight = ALTOIMAGEN - 10;

	// Usamos las coordenadas de celda almacenadas para mayor precisi�n
	int filaActual = cellY;
	int columnaActual = cellX;

	// Calculamos la posici�n exacta del centro de la explosi�n
	int centroX = columnaActual * ANCHOIMAGEN + (ANCHOIMAGEN - explosionWidth) / 2;
	int centroY = filaActual * ALTOIMAGEN + (ALTOIMAGEN - explosionHeight) / 2;

	// Dibujamos el centro de la explosion
	Rectangle centro = Rectangle(centroX, centroY, explosionWidth, explosionHeight);
	Rectangle posicionCentro = Rectangle(indiceEX * ANCHOEXPLOSION, indiceEY * ALTOEXPLOSION, ANCHOEXPLOSION, ALTOEXPLOSION);
	g->DrawImage(bmpExplosion, centro, posicionCentro, GraphicsUnit::Pixel);

	// Definimos los sprites para las diferentes partes de la explosion
	Rectangle izquierdaDerecha = Rectangle(indiceEX * ANCHOEXPLOSION, indiceEY + 2 * ALTOEXPLOSION, ANCHOEXPLOSION, ALTOEXPLOSION);
	Rectangle arribaAbajo = Rectangle(indiceEX * ANCHOEXPLOSION, indiceEY + 6 * ALTOEXPLOSION, ANCHOEXPLOSION, ALTOEXPLOSION);

	// Verificamos y dibujamos la explosion a la izquierda
	if (columnaActual > 0 && cancha[filaActual][columnaActual - 1]->getTipo() != EElementos::paredes) {
		Rectangle izquierda = Rectangle(centroX - ANCHOIMAGEN, centroY, explosionWidth, explosionHeight);
		g->DrawImage(bmpExplosion, izquierda, izquierdaDerecha, GraphicsUnit::Pixel);
		if (cancha[filaActual][columnaActual - 1]->getTipo() == EElementos::rrompible) {
			cancha[filaActual][columnaActual - 1]->setTipo(EElementos::libre);
		}
	}

	// Verificamos y dibujamos la explosion a la derecha
	if (columnaActual < COLUMNAS - 1 && cancha[filaActual][columnaActual + 1]->getTipo() != EElementos::paredes) {
		Rectangle derecha = Rectangle(centroX + ANCHOIMAGEN, centroY, explosionWidth, explosionHeight);
		g->DrawImage(bmpExplosion, derecha, izquierdaDerecha, GraphicsUnit::Pixel);
		if (cancha[filaActual][columnaActual + 1]->getTipo() == EElementos::rrompible) {
			cancha[filaActual][columnaActual + 1]->setTipo(EElementos::libre);
		}
	}

	// Verificamos y dibujamos la explosion arriba
	if (filaActual > 0 && cancha[filaActual - 1][columnaActual]->getTipo() != EElementos::paredes) {
		Rectangle arriba = Rectangle(centroX, centroY - ALTOIMAGEN, explosionWidth, explosionHeight);
		g->DrawImage(bmpExplosion, arriba, arribaAbajo, GraphicsUnit::Pixel);
		if (cancha[filaActual - 1][columnaActual]->getTipo() == EElementos::rrompible) {
			cancha[filaActual - 1][columnaActual]->setTipo(EElementos::libre);
		}
	}

	// Verificamos y dibujamos la explosion abajo
	if (filaActual < FILAS - 1 && cancha[filaActual + 1][columnaActual]->getTipo() != EElementos::paredes) {
		Rectangle abajo = Rectangle(centroX, centroY + ALTOIMAGEN, explosionWidth, explosionHeight);
		g->DrawImage(bmpExplosion, abajo, arribaAbajo, GraphicsUnit::Pixel);
		if (cancha[filaActual + 1][columnaActual]->getTipo() == EElementos::rrompible) {
			cancha[filaActual + 1][columnaActual]->setTipo(EElementos::libre);
		}
	}
}