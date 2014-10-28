//Author: Leopoldo Pla

import java.util.Stack;
import java.util.Vector;

public class AnalizadorSintacticoSLR {

	public class Celda {
		public int tipo;
		public int destino;

		public static final int R = 1, D = 2, A = 3;

		public Celda(int tipo, int estado) {
			this.tipo = tipo;
			this.destino = estado;
		}
	}

	private AnalizadorLexico al;
	private Celda[][] accion;
	private int[][] ir_a;
	private Stack<Integer> pila;
	private Token a;
	private int s;

	public AnalizadorSintacticoSLR(AnalizadorLexico al) {
		this.al = al;
		pila = new Stack<Integer>();

		accion = new Celda[43][16];

		accion[0][9] = new Celda(Celda.D, 3);
		accion[0][10] = new Celda(Celda.D, 4);
		accion[1][15] = new Celda(Celda.A, 0);
		accion[2][12] = new Celda(Celda.D, 5);
		accion[3][3] = new Celda(Celda.R, 2);
		accion[3][12] = new Celda(Celda.R, 2);
		accion[4][3] = new Celda(Celda.R, 3);
		accion[4][12] = new Celda(Celda.R, 3);
		accion[5][0] = new Celda(Celda.D, 7);
		accion[6][4] = new Celda(Celda.D, 9);
		accion[7][1] = new Celda(Celda.R, 6);
		accion[7][9] = new Celda(Celda.D, 3);
		accion[7][10] = new Celda(Celda.D, 4);
		accion[8][15] = new Celda(Celda.R, 1);
		accion[9][1] = new Celda(Celda.R, 8);
		accion[9][4] = new Celda(Celda.D, 9);
		accion[9][9] = new Celda(Celda.D, 3);
		accion[9][10] = new Celda(Celda.D, 4);
		accion[9][11] = new Celda(Celda.D, 17);
		accion[9][12] = new Celda(Celda.D, 16);
		accion[10][1] = new Celda(Celda.D, 19);
		accion[11][2] = new Celda(Celda.D, 21);
		accion[11][1] = new Celda(Celda.R, 8);
		accion[12][3] = new Celda(Celda.D, 22);
		accion[12][12] = new Celda(Celda.D, 23);
		accion[13][5] = new Celda(Celda.D, 24);
		accion[14][4] = new Celda(Celda.D, 9);
		accion[14][5] = new Celda(Celda.R, 13);
		accion[14][9] = new Celda(Celda.D, 3);
		accion[14][10] = new Celda(Celda.D, 4);
		accion[14][11] = new Celda(Celda.D, 17);
		accion[14][12] = new Celda(Celda.D, 16);
		accion[15][4] = new Celda(Celda.R, 14);
		accion[15][5] = new Celda(Celda.R, 14);
		accion[15][9] = new Celda(Celda.R, 14);
		accion[15][10] = new Celda(Celda.R, 14);
		accion[15][11] = new Celda(Celda.R, 14);
		accion[15][12] = new Celda(Celda.R, 14);
		accion[16][6] = new Celda(Celda.D, 25);
		accion[17][0] = new Celda(Celda.D, 26);
		accion[18][12] = new Celda(Celda.D, 27);
		accion[19][4] = new Celda(Celda.R, 4);
		accion[20][1] = new Celda(Celda.R, 5);
		accion[21][9] = new Celda(Celda.D, 3);
		accion[21][10] = new Celda(Celda.D, 4);
		accion[22][12] = new Celda(Celda.D, 29);
		accion[23][1] = new Celda(Celda.R, 10);
		accion[23][2] = new Celda(Celda.R, 10);
		accion[24][4] = new Celda(Celda.R, 11);
		accion[24][5] = new Celda(Celda.R, 11);
		accion[24][9] = new Celda(Celda.R, 11);
		accion[24][10] = new Celda(Celda.R, 11);
		accion[24][11] = new Celda(Celda.R, 11);
		accion[24][12] = new Celda(Celda.R, 11);
		accion[24][15] = new Celda(Celda.R, 11);
		accion[25][13] = new Celda(Celda.D, 32);
		accion[25][14] = new Celda(Celda.D, 33);
		accion[26][1] = new Celda(Celda.R, 8);
		accion[26][13] = new Celda(Celda.D, 32);
		accion[26][14] = new Celda(Celda.D, 33);
		accion[27][7] = new Celda(Celda.D, 35);
		accion[28][1] = new Celda(Celda.R, 8);
		accion[28][2] = new Celda(Celda.D, 21);
		accion[29][1] = new Celda(Celda.R, 9);
		accion[29][2] = new Celda(Celda.R, 9);
		accion[30][7] = new Celda(Celda.D, 37);
		accion[30][8] = new Celda(Celda.D, 38);
		accion[31][1] = new Celda(Celda.R, 19);
		accion[31][7] = new Celda(Celda.R, 19);
		accion[31][8] = new Celda(Celda.R, 19);
		accion[32][1] = new Celda(Celda.R, 20);
		accion[32][7] = new Celda(Celda.R, 20);
		accion[32][8] = new Celda(Celda.R, 20);
		accion[33][1] = new Celda(Celda.R, 21);
		accion[33][7] = new Celda(Celda.R, 21);
		accion[33][8] = new Celda(Celda.R, 21);
		accion[34][1] = new Celda(Celda.D, 39);
		accion[34][8] = new Celda(Celda.D, 38);
		accion[35][4] = new Celda(Celda.R, 17);
		accion[35][5] = new Celda(Celda.R, 17);
		accion[35][9] = new Celda(Celda.R, 17);
		accion[35][10] = new Celda(Celda.R, 17);
		accion[35][11] = new Celda(Celda.R, 17);
		accion[35][12] = new Celda(Celda.R, 17);
		accion[36][1] = new Celda(Celda.R, 7);
		accion[37][4] = new Celda(Celda.R, 15);
		accion[37][5] = new Celda(Celda.R, 15);
		accion[37][9] = new Celda(Celda.R, 15);
		accion[37][10] = new Celda(Celda.R, 15);
		accion[37][11] = new Celda(Celda.R, 15);
		accion[37][12] = new Celda(Celda.R, 15);
		accion[38][13] = new Celda(Celda.D, 32);
		accion[38][14] = new Celda(Celda.D, 33);
		accion[39][4] = new Celda(Celda.D, 9);
		accion[39][9] = new Celda(Celda.D, 3);
		accion[39][10] = new Celda(Celda.D, 4);
		accion[39][11] = new Celda(Celda.D, 17);
		accion[39][12] = new Celda(Celda.D, 16);
		accion[40][1] = new Celda(Celda.R, 18);
		accion[40][7] = new Celda(Celda.R, 18);
		accion[40][8] = new Celda(Celda.R, 18);
		accion[41][4] = new Celda(Celda.R, 16);
		accion[41][5] = new Celda(Celda.R, 16);
		accion[41][9] = new Celda(Celda.R, 16);
		accion[41][10] = new Celda(Celda.R, 16);
		accion[41][11] = new Celda(Celda.R, 16);
		accion[41][12] = new Celda(Celda.R, 16);
		accion[42][5] = new Celda(Celda.R, 12);

		ir_a = new int[43][11];

		ir_a[0][0] = 1;
		ir_a[0][1] = 2;
		ir_a[5][2] = 6;
		ir_a[6][3] = 8;
		ir_a[7][1] = 12;
		ir_a[7][4] = 10;
		ir_a[7][5] = 11;
		ir_a[9][1] = 18;
		ir_a[9][3] = 15;
		ir_a[9][7] = 13;
		ir_a[9][8] = 14;
		ir_a[11][6] = 20;
		ir_a[14][1] = 18;
		ir_a[14][3] = 15;
		ir_a[14][7] = 42;
		ir_a[14][8] = 14;
		ir_a[21][1] = 12;
		ir_a[21][5] = 28;
		ir_a[25][9] = 30;
		ir_a[25][10] = 31;
		ir_a[26][9] = 34;
		ir_a[26][10] = 31;
		ir_a[28][6] = 36;
		ir_a[38][10] = 40;
		ir_a[39][1] = 18;
		ir_a[39][3] = 15;
		ir_a[39][8] = 41;
	}

	public void analizar() {
		Stack<Integer> pilaSolucion = new Stack<Integer>();

		pila.push(0);
		a = al.siguienteToken();

		do {
			s = pila.peek();
			if (accion[s][a.tipo - 1] == null)
				error();
			else if (accion[s][a.tipo - 1].tipo == Celda.D) {
				pila.push(accion[s][a.tipo - 1].destino);
				a = al.siguienteToken();
			} else if (accion[s][a.tipo - 1].tipo == Celda.R) {
				for (int i = 0; i < Longitud_Parte_Derecha(accion[s][a.tipo - 1].destino); i++) {
					pila.pop();
				}
				pilaSolucion.push(accion[s][a.tipo - 1].destino);

				int p = pila.peek();
				int A = Parte_Izquierda(accion[s][a.tipo - 1].destino);

				pila.push(ir_a[p][A]);
			} else if (accion[s][a.tipo - 1].tipo == Celda.A) {
				break;
			}

		} while (true);

		while (!pilaSolucion.empty()) {
			System.out.print(pilaSolucion.pop() + " ");
		}
	}

	private int Parte_Izquierda(int regla) {
		switch (regla) {
		case 1:
			return 0;
		case 2:
			return 1;
		case 3:
			return 1;
		case 4:
			return 2;
		case 5:
			return 4;
		case 6:
			return 4;
		case 7:
			return 6;
		case 8:
			return 6;
		case 9:
			return 5;
		case 10:
			return 5;
		case 11:
			return 3;
		case 12:
			return 7;
		case 13:
			return 7;
		case 14:
			return 8;
		case 15:
			return 8;
		case 16:
			return 8;
		case 17:
			return 8;
		case 18:
			return 9;
		case 19:
			return 9;
		case 20:
			return 10;
		case 21:
			return 10;
		}
		return 0;
	}

	private int Longitud_Parte_Derecha(int regla) {
		switch (regla) {
		case 1:
			return 4;
		case 2:
			return 1;
		case 3:
			return 1;
		case 4:
			return 3;
		case 5:
			return 2;
		case 6:
			return 0;
		case 7:
			return 3;
		case 8:
			return 0;
		case 9:
			return 3;
		case 10:
			return 2;
		case 11:
			return 3;
		case 12:
			return 2;
		case 13:
			return 1;
		case 14:
			return 1;
		case 15:
			return 4;
		case 16:
			return 5;
		case 17:
			return 3;
		case 18:
			return 3;
		case 19:
			return 1;
		case 20:
			return 1;
		case 21:
			return 1;
		}
		return 0;
	}

	private void error() {
		if (a.tipo == Token.EOF)
			System.err
					.print("Error sintactico: encontrado el final del fichero, esperaba");
		else
			System.err.print("Error sintactico (" + a.fila + "," + a.columna
					+ "): encontrado '" + a.lexema + "'" + ", esperaba");

		Vector<Integer> tokEsperado = new Vector<Integer>();
		for (int i = 0; i < 16; i++) {
			if (accion[s][i] != null)
				tokEsperado.add(i);
		}

		for (int tok : tokEsperado) {
			Token tmp = new Token();
			tmp.tipo = tok + 1;
			System.err.print(" " + tmp.toString());
		}

		System.err.println();
		System.exit(-1);
	}
}
