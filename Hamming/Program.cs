/*
 * Stefan Niculae, 231
 * 16 nov 2014
 * Tema 2 Laborator Retele de Calculatoare
 * 
 * Codificarea Hamming
 * 
 * 
 * Primim un numar de biti
 * Vedem de ce codificare avem nevoie H(7,4), H(15, 11), H(31,26) etc
 * Inseram bitii de paritate si intoarcem codul Hamming obtinut
 * 
 * Pentru a corecta un cod primit
 * verificam paritatea apoi extragem informatia utila
 * 
 * 
 * Cea mai grea parte in implementare a fost 
 * gasirea unei formule pentru m, din relatia Hamming (2^m - m, 2^m - m - 1)
 * in functie numai de lungimea input-ului
 */


using System;
using System.IO;
using System.Text;

namespace Hamming
{
	class MainClass
	{
		static string data, HammCode;

		public static void Main (string[] args)
		{
			//readData ();

			data = "101010";

			insertParityBits ();

			char[] damagedCode = HammCode.ToCharArray ();

			damagedCode [0] = '1';

			correctCode (new string (damagedCode));
		}

		static void readData ()
		{
			Console.Write ("Enter bits to send: ");
			data = Console.ReadLine ();

			foreach (char letter in data)
				if (letter != '0' && letter != '1') {
					Console.WriteLine ("Invalid format ({0}), try again!", letter);
					readData ();
				}

			if (data.Length < 4) {
				Console.WriteLine ("Please enter more than 3 bits!");
				readData ();
			}
		}

		static void insertParityBits ()
		{
			StringBuilder code = new StringBuilder (data);
			// Number of bits to add
			int m = (int)(Math.Log (code.Length + 1 + Math.Log (code.Length, 2), 2)) + 1;
			Console.WriteLine ("Using Hamming ({0}, {1})", (1 << m) - 1, (1 << m) - m - 1);

			// Inserting parity bits (initially set to zero)
			int power = 1;
			for (int i = 0; i < m; i++) {
				code = code.Insert (power - 1, "0");
				power <<= 1;
			}

			power = 1;
			for (int i = 0; i < m; i++) {

				Console.Write ("P{0} = ", power);
				int parity = 0;
				string toPrint = "";

				for (int index = 1, indexInData = 1; index <= code.Length; index++) {

					// No need to add zeroes initialized above (if the index is not a power of two)
					if ((index & (index - 1)) != 0) {

						if ((power & index) == power) {

							toPrint += String.Format ("D{0}({1}) + ", indexInData, code [index - 1]);
							parity = (parity + (int)(code [index - 1] - '0')) % 2;

						}

						indexInData++;
					}
				}

				toPrint = toPrint.Substring (0, toPrint.Length - 3);
				Console.WriteLine ("{0} = {1}", toPrint, parity);

				if (parity == 1)
					code [power - 1] = '1';

				power <<= 1;


			}

			Console.WriteLine ("\nEncoded data: {0}", code);
			HammCode = code.ToString();
		}

		static void correctCode (string code) 
		{
			int m = (int)Math.Log (code.Length, 2) + 1;
			short[] damagedIndex = new short[m];
			damagedIndex.Initialize ();

			Console.WriteLine ("\n\nReceived code = {0}, in format ({1}, {2})", code, (1 << m) - 1, (1 << m) - m - 1);

			int power = 1;
			for (int i = 0; i < m; i++) {

				string toPrint = "";

				for (int index = 1, indexInData = 1; index <= code.Length; index++) {

					if ((power & index) == power) {

						if ((index & (index - 1)) == 0)
							toPrint += String.Format ("P{0}({1}) + ", power, code [index - 1]);

						else
							toPrint += String.Format ("D{0}({1}) + ", indexInData, code [index - 1]);

						damagedIndex[i] = (short)((damagedIndex[i] + (code [index - 1] - '0')) % 2);

					}

					if ((index & (index - 1)) != 0)
						indexInData++;
				}


				toPrint = toPrint.Substring (0, toPrint.Length - 3);
				Console.WriteLine ("{0} = {1}", toPrint, damagedIndex[i]);

				power <<= 1;

			}


			string reversedIndex = "";
			for (int i = damagedIndex.Length - 1; i >= 0; i--)
				reversedIndex += damagedIndex [i];
			int indexInDecimal = Convert.ToInt32 (reversedIndex, 2);

			Console.WriteLine ("Damaged index \n\tin binary = {0} \n\tin decimal = {1}", reversedIndex, indexInDecimal);


			char[] correctedCode = code.ToCharArray ();
			if (indexInDecimal == 0)
				Console.WriteLine ("The code is correct! (or it has more than one error)");
			else {
				if (correctedCode [indexInDecimal - 1] == '0')
					correctedCode [indexInDecimal - 1] = '1';
				else
					correctedCode [indexInDecimal - 1] = '0';

				Console.WriteLine ("Corrected code = {0}", new string (correctedCode));
			}


			string decodedData = "";
			// Take only the bits on positions that are not a power of two
			for (int i = 0; i < correctedCode.Length; i++)
				if ((i + 1 & (i)) != 0)
					decodedData += correctedCode [i];

			Console.WriteLine ("\nDecoded data = {0}", decodedData);

		}
	}
}
