#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<dirent.h>
#define Max_path_name 200
#define Max_class_name 40 
#define Max_Matiere_name 20
#define Max_Eleve_name 20
#define Max_notes_name 20


#define STR_(X) #X     // convert a number into a str
#define STR(X) STR_(X) 


int length(char* l) { // verifie la taille d'un string dans un tableau 
	int s = 0;
	while (*l != '\0') {
		l++;
		s++;

	}
	return s;

}
int CheckInput(char* Input) {

	int i;
	if (*Input == '.') { printf("\n   --Le premier Character ne peut pas etre un point !-- \n\n"); return -1; }
	while (*Input != '\0') {
		if (*Input == '/' || *Input == '\\' || *Input == '?' || *Input == '*' || *Input == ':' || *Input == '<' || *Input == '>' || *Input == '\"' || *Input == '|' || *Input == '?') {
			printf("\n    -- Veuillez ne pas utiliser les Character suivant :\n          \\ / : * ? \" < > |\n\n");
			return -1;
		}

		i++;
		Input++;
	}
	return 1;
}

float getvalnote(char matiere[(Max_Matiere_name + 1)], char Class[(Max_class_name + 1)], char Eleve[(Max_Eleve_name + 2)], int print) {
	char path[Max_path_name];

	char filename[Max_path_name] = "SCHOOL/";
	DIR* d;
	struct dirent* dir;
	strcat(filename, Class);
	strcat(filename, "/");						//construit le path pour un eleve
	strcat(filename, matiere);

	if (!(d = opendir(filename))) {
		printf("\nErreur   --Cette Matiere n'existe pas--\n\n");
		return -2;
	}
	closedir(d);
	strcat(filename, "/");
	strcat(filename, Eleve);
	strcat(filename, "/");
	for (int i = 0; i < Max_path_name; i++) {
		path[i] = filename[i];                                    //sauvegarde le path dans path     
	}

	//ouvre le path
	char lstNotes[110][Max_notes_name + 1];
	char lstModifNotes[110][Max_notes_name + 1];

	int NbOfnote = 0;
	int c = 0;
	int flag;
	d = opendir(filename);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0)) { continue; }
			else {
				for (int i = 0; i < Max_path_name; i++) {        //remet le path a zero 
					filename[i] = path[i];
				}
				strcpy(lstNotes[NbOfnote], dir->d_name);              // sauvegarde toute les note d'un eleve pour une matiere dans un tableau
				if (print == -3) { remove(strcat(filename, lstNotes[NbOfnote])); }


				c = (Max_notes_name);
				flag = 0;
				while (c >= 0) {
					if (lstNotes[NbOfnote][c] != '.' && flag == 0) {          // sauvegarde toute les note sans le .txt dans un tableau

						lstModifNotes[NbOfnote][c] = '\0';
					}
					else if (flag == 0) {
						flag = 1;
						lstModifNotes[NbOfnote][c] = '\0';

					}
					else {
						lstModifNotes[NbOfnote][c] = lstNotes[NbOfnote][c];
					}
					c--;
				}

				NbOfnote++;
			}
		}
		closedir(d);				//ferme le fichier
	}
	else if (print == -3) { return -3; }
	else { // si le fichier n'est pas ouvrable retourn une erreur
		printf("\nErreur   --Cette Eleve n'existe pas--\n\n");
		return -2;

	}


	char note[9]; // la note est au maxium un float egale a 20.000000 donc pas besoin d'un string plus grand
	FILE* ftr;
	float moyenne = 0;
	float sum = 0.0;
	float nbn = 0.0;  // nombre de note d'une matiere
	float test;
	char* err;
	if (print == 2) { printf("\n"); }
	for (int u = 0; u < NbOfnote; u++) {
		for (int i = 0; i < Max_path_name; i++) {        //remet le path a zero pour ouvrire la deuxieme note 
			filename[i] = path[i];

		}



		ftr = fopen((strcat(filename, lstNotes[u])), "r+");    //ouvre la premiere note de la liste
		if (ftr != NULL) {
			fgets(note, sizeof(note), ftr);

			if (print == 2) {
				printf("  --%s :", (lstModifNotes[u]));           //affiche les note si demander
				printf(" %s", note);
				printf("\n");
			}

			test = strtod(note, &err);           //verifie si les notes sont des nombre

			if (note == err) {    // si non les retire du calcul
				nbn--;
			}
			sum += test;       // additione toute les notes
			fclose(ftr);

			nbn++;
		}
	}

	if (nbn == 0) {
		if (print == 0 || print == 2) {
			printf("  --Aucune Notes--\n"); // si il y a aucune note 
			return -1;
		}
		return -1;
	}
	moyenne = sum / nbn;  // calcule la moyenne d'une matiere d'une eleve
	if (print == 0) {
		printf("          --%.2f--\n\n", moyenne);
	}
	return moyenne;


}
void modifynote(char matiere[(Max_Matiere_name + 1)], char Class[(Max_class_name + 1)], char Eleve[(Max_Eleve_name + 2)]) {
	char filename[Max_path_name] = "SCHOOL/";
	char nomdelanote[(Max_notes_name + 1)];
	float note = -1;
	strcat(filename, Class);
	strcat(filename, "/");						//construit le path pour un eleve d'une matiere
	strcat(filename, matiere);
	strcat(filename, "/");
	strcat(filename, Eleve);
	strcat(filename, "/");


	printf("enter le nom de la note :\n");
	scanf("%"STR(Max_notes_name)"s", &nomdelanote);
	if (CheckInput(nomdelanote) < 0) { return; }
	strcat(filename, nomdelanote);
	scanf("%*[^\n]");
	printf("entrer la note :\n");
	scanf("%f", &note);
	scanf("%*[^\n]");
	if (note <= 20 && note >= 0) { // verifie que la nouvelle note est entre 0 et 20
		FILE* ftr = fopen(strcat(filename, ".txt"), "w+"); // ouvre la note
		fprintf(ftr, "%f", note); // print la note dans le fichier txt
		fclose(ftr);
		printf("\n  --modification effectuer--\n\n");
	}
	else {
		printf("\n La note doit etre un Nombre entre 0-20\n");
	}
}
void delnote(char matiere[(Max_Matiere_name + 1)], char Class[(Max_class_name + 1)], char Eleve[(Max_Eleve_name + 2)]) {
	char filename[Max_path_name] = "SCHOOL/";
	char nomdelanote[(Max_notes_name + 1)];
	strcat(filename, Class);
	strcat(filename, "/");						//construit le path de la matiere de l'eleve
	strcat(filename, matiere);
	strcat(filename, "/");
	strcat(filename, Eleve);
	strcat(filename, "/");

	printf("enter le nom de la note :\n");
	scanf("%"STR(Max_notes_name)"s", &nomdelanote);
	if (CheckInput(nomdelanote) < 0) { return; }
	scanf("%*[^\n]");
	strcat(filename, nomdelanote);
	remove(strcat(filename, ".txt"));  // supprime la note 

	printf("\n  --Supression effectuer--\n\n");

}
void doModif(char matiere[(Max_Matiere_name + 1)], char Class[(Max_class_name + 1)], char Eleve[(Max_Eleve_name + 2)]) {// menu pour ajouter ou supprimer les notes d'un eleve d'une matiere
	int modif;
	if (getvalnote(matiere, Class, Eleve, 2) == -2) { return; }  // verifie si la matiere existe et si l'eleve existe aussi au pssage affiche les notes de la matiere
	while (1 == 1) {
		modif = -1;
		printf("\ntappez 0 pour retourner en arriere, \ntappez 1 pour modifier/ajouter une note,\ntappez 2 pour supprimer une note \n");
		scanf(" %d", &modif);
		scanf("%*[^\n]");
		if (modif == 0) { break; }
		if (modif == 1) {
			modifynote(matiere, Class, Eleve); // pour ajouter ou modifier une note 
			getvalnote(matiere, Class, Eleve, 2); // reaffiche les notes
		}
		else if (modif == 2) {
			delnote(matiere, Class, Eleve); // pour suprimer une note
			getvalnote(matiere, Class, Eleve, 2); // reaffiche les notes

		}
		else {
			printf("\n --Veuillez ressayer--\n\n");


		}
	}


}

int getmatlst();
int showlstmat();
int getclasslst();


float moyenneGofEleve(char Class[(Max_class_name + 1)], char Eleve[(Max_Eleve_name + 2)], int print) { // moyenne d'un eleve
	char lstMat[110][(Max_Matiere_name + 1)];
	int NbOfMat = getmatlst(Class, &lstMat[0][0]);
	float sum = 0.0;
	float moyenne;
	float test;
	float tst = 0.0;
	for (int u = 0; u < NbOfMat; u++) { //boucle pour avoir toutes moyenne de toute les matiere
		if (print == 2) { // mode pour tout print

			printf("\n         ---%s---", lstMat[u]); // print le nom de la matiere
			getvalnote(lstMat[u], Class, Eleve, 2);  // print toutes les notes au passage

		}



		else { // calcul la moyenne g
			test = getvalnote(lstMat[u], Class, Eleve, 1);
			if (test != -1 && test != -2) {
				sum += test;
				tst++;
			}
		}

	}

	if (print == 2) { // ne rien retourner
		return 0;
	}

	if (tst == 0 && print != 4) {
		printf("  --Aucune Notes--\n");
		return -1;
	}

	moyenne = (float)sum / tst; // retourner la moyenne pour la print ou l'utiliser dans une autre fonction
	return moyenne;
}
int checkEleve(char Class[(Max_class_name + 1)], char Eleve[(Max_Eleve_name + 2)], int print) {
	char filename[Max_path_name] = "SCHOOL/";
	DIR* d;
	struct dirent* dir;
	strcat(filename, Class);
	d = opendir(filename);
	if (!d) { printf("\n  --Cette class existe pas\n\n"); return-1; } // veriife si la class existe
	closedir(d);


	strcat(filename, "/...eleve.txt");  // ouvre la liste des eleves
	FILE* ftr = fopen(filename, "a+");
	char eleve[110][(Max_Eleve_name + 2)];
	int i = 0;
	printf("\n");
	while (fgets(eleve[i], sizeof(eleve), ftr) != NULL) { // sauvegarde la liste des eleves
		eleve[i][(length(eleve[i]) - 1)] = '\0';
		if (print == 1) {
			printf("  --%s\n", eleve[i]);  //montrer tous les eleves 
		}
		else if (print == 4) { // calcule la moyenne generale de chaque eleve en les montrant
			printf("  --%s : ", eleve[i]);
			if (moyenneGofEleve(Class, eleve[i], 0) > 0) {
				printf("%.2f/20\n", moyenneGofEleve(Class, eleve[i], 0));
			}
		}

		i++;
	}
	fclose(ftr);
	int pos;

	if (Eleve != NULL) { // si un eleve a ete preciser regarde si il existe dans la liste
		int flag = 0;
		int Elevefound = 0;

		for (int h = 0; h < i; h++) {
			for (int t = 0; t <= length(Eleve) && t <= length(eleve[h]); t++) {//boucle pour comparer chaque character de chaque eleve

				if (eleve[h][t] != Eleve[t]) {
					Elevefound = 0;

					break;
				}
				else {
					Elevefound = 1;

				}
			}
			if (Elevefound == 1) {
				flag = 1;
				if (print == -2) {
					for (int u = 0; u < (Max_Eleve_name + 2); u++) { //bouccle pour  supprimer l'eleve  preciser de la liste
						eleve[h][u] = '\0';

					}
					pos = h; // sauvegarde son index dans la variable pos pour reecrire toute la liste sans lui
				}
				if (print == 2) {
					printf("\n  --Cette Eleve existe deja--\n\n");
					return 2;
				}

			}
		}
		if (flag != 1 && print != 2) {
			printf("\n  --Eleve non trouvee--\n\n");
			return -2;

		}

	}
	else {
		if (i == 0 && print > 0) { printf("   --Aucun Eleve--\n");  return-2; }
	}

	if (print == -2) { // mode pour supprimer l'eleve
		ftr = fopen(filename, "w+");
		for (int f = 0; f < i; f++) { //boucle pour reecrire la nouvelle liste sans l'eleve preciser
			if (f != pos) {
				fprintf(ftr, "%s\n", eleve[f]);


			}
		}
		fclose(ftr);
	}
	return 0;
}
int ShowEleve(char Class[(Max_class_name + 1)], char Eleve[(Max_Eleve_name + 2)]) {
	int choix = -1;
	while (choix != 0) {

		choix = -1;
		printf("            --Que voulez vous savoir a propos de %s de %s ? \n(Tappez 0) Pour retourner en arriere \n(Tappez 1) Voir ca moyenne Generale  \n(Tappez 2) Voir ca Moyenne dans une Matiere  \n(Tappez 3) Voir/Modifier ses notes dans une Matiere  \n(Tappez 4) Voir/Modifier ses notes de toutes ses matieres  \n", Eleve, Class);
		scanf("%*[^\n]");
		scanf("%d", &choix);
		scanf("%*[^\n]");
		if (choix == 0) {}
		else if (choix == 1) {
			float result = moyenneGofEleve(Class, Eleve, 0);
			if (result != -1) { printf("         --%.2f--\n\n", result); }  // print la moyenne generale de l'eleve mentionner seulement si la moyenne n'a pas retourner d'erreur

		}
		else if (choix == 2) {
			if (showlstmat(Class, 1) != -1) {
				char matiere[(Max_Matiere_name + 1)];
				printf("\nIndiquez le nom de la Matiere :\n");
				scanf("%"STR(Max_Matiere_name)"s", &matiere);
				if (CheckInput(matiere) < 0) { return -1; }
				scanf("%*[^\n]");
				getvalnote(matiere, Class, Eleve, 0); // avoir la moyenne d'une matiere de l'eleve 

			}
			else { printf("   --Aucune Matiere--\n"); }

		}
		else if (choix == 3) {
			char matiere[(Max_Matiere_name + 1)];

			if (showlstmat(Class, 1) != -1) {
				printf("\nIndiquez le nom de la Matiere a voir :\n");
				scanf("%"STR(Max_Matiere_name)"s", &matiere);
				if (CheckInput(matiere) < 0) { return -1; }
				scanf("%*[^\n]");
				doModif(matiere, Class, Eleve); // pour modifier/ajouter les notes de l'eleve dans une matiere
			}
			else { printf("   --Aucune Matiere--\n"); }
		}
		else if (choix == 4) {
			moyenneGofEleve(Class, Eleve, 2);  // affiche toute les notes par matiere
			int modif = 0;
			printf("\ntappez 0 pour retourner en arriere, \ntappez 1 pour modifier les notes d'une Matiere,\n");
			scanf(" %d", &modif);
			scanf("%*[^\n]");
			if (modif == 1) {
				char matiere[(Max_Matiere_name + 1)];

				printf("\nIndiquez le nom de la Matiere  :\n");
				scanf("%"STR(Max_Matiere_name)"s", &matiere);
				if (CheckInput(matiere) < 0) { return -1; }
				scanf("%*[^\n]");
				doModif(matiere, Class, Eleve); // poour modifier les notes dune matiere

			}
		}
		else {
			printf("\n\n        --Veuillez ressayez--\n\n");
			scanf("%*[^\n]");

		}

	}
}
int AddEleve(char Class[(Max_class_name + 1)], char  Eleve[(Max_Eleve_name + 2)], int mod, char newEleve[(Max_Eleve_name + 2)]) {

	if (Eleve == NULL) { printf("EREEEEUUUR"); return -1; }
	char filename[Max_path_name] = "SCHOOL/";
	char path[Max_path_name];
	strcat(filename, Class);
	strcat(filename, "/");						//sauvegarde le path de la class pour le reutiliser apres un strcat
	for (int i = 0; i < Max_path_name; i++) {
		path[i] = filename[i];
	}
	DIR* d;
	struct dirent* dir;
	d = opendir(filename);
	int NbOfmat = 0;
	char lstMat[110][Max_Matiere_name];
	if (d)
	{

		printf("\n");
		while ((dir = readdir(d)) != NULL) {      // fait une boucle pour ajouter l'eleve dans toute les matieres 

			if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0) || (strcmp(dir->d_name, "...eleve.txt") == 0)) {// pour eviter de confondre  (".","..","...eleve.txt")   avec une  matiere
				continue;
			}
			else {         // pour eviter de confondre  (".","..","...eleve.txt")   avec une  matiere
				strcpy(lstMat[NbOfmat], dir->d_name);
				strcat(filename, lstMat[NbOfmat]);
				strcat(filename, "/");
				if (mod == 1) { // ajoute le repertoir de l'eleve a la matiere 
					mkdir(strcat(filename, Eleve));
				}
				if (mod == 2) {      // renomme l'eleve par le nouveau nom 
					char  path2[Max_path_name];
					for (int i = 0; i < Max_path_name; i++) { // sauvegarde le path pour le renommer avec deux strcat juste en dessous
						path2[i] = filename[i];
					}
					rename(strcat(filename, Eleve), strcat(path2, newEleve));
				}
				NbOfmat++;
			}

			for (int i = 0; i < Max_path_name; i++) {
				filename[i] = path[i];                    // remet le path a zero pour les autre matieres                    
			}
		}
		closedir(d);
	}


	if (mod == 1) {//ajouter un eleve
		strcat(filename, "/...eleve.txt");
		FILE* ftr = fopen(filename, "a");
		fprintf(ftr, "%s\n", Eleve);   // ajoute le nouveau eleve en bas de la liste

		fclose(ftr);
	}
	if (mod == 2) {//renommer un eleve
		checkEleve(Class, Eleve, -2);  //supprime l'ancien nom de la liste 
		strcat(filename, "/...eleve.txt");
		FILE* ftr = fopen(filename, "a");
		fprintf(ftr, "%s\n", newEleve);  // ajoute le nouveau nom 

		fclose(ftr);


	}


}
int DelEleve(char Class[(Max_class_name + 1)], char Eleve[(Max_Eleve_name + 2)]) {
	char filename[Max_path_name] = "SCHOOL/";
	char path[Max_path_name];
	strcat(filename, Class);
	strcat(filename, "/");
	for (int i = 0; i < Max_path_name; i++) {
		path[i] = filename[i];                                    //sauvegarde le path de la class pour a reutiliser apres un strcat
	}

	DIR* d;
	struct dirent* dir;
	d = opendir(filename);
	int NbOfMat = 0;
	char lstMat[110][(Max_Matiere_name + 1)];
	if (d)
	{

		printf("\n");
		while ((dir = readdir(d)) != NULL) {  // boucle pour avoir la liste des matiere pour pouvoir supprimer l'eleve de toute les matiere

			if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0) || (strcmp(dir->d_name, "...eleve.txt") == 0)) {// pour eviter de confondre  (".","..","...eleve.txt")   avec une  matiere
				continue;
			}
			else {
				strcpy(lstMat[NbOfMat], dir->d_name);
				getvalnote(lstMat[NbOfMat], Class, Eleve, -3);   // supprime toute les note de l'eleve dans une matiere
				strcat(filename, lstMat[NbOfMat]);
				strcat(filename, "/");
				rmdir(strcat(filename, Eleve));    // supprime le fichier de l'eleve dans la matiere 

			}

			for (int i = 0; i < Max_path_name; i++) {
				filename[i] = path[i];                                    //remets le path de la class pour les autres  Matieres 
			}
			NbOfMat++;
		}
		closedir(d);

	}
	checkEleve(Class, Eleve, -2);    // retire l'eleve de la liste des eleves
}
void modifEleve(char Class[(Max_class_name + 1)]) {   /// c'est juste un menu qui selon ton choix fait differente fonction

	while (1 == 1) {
		int choix = -1;
		printf("      --Eleves--");
		checkEleve(Class, NULL, 1);
		printf("\n(Tappez 0) Pour retourner en arriere \n(Tappez 1) Ajouter un Eleve  \n(Tappez 2) Renommer un Eleve  \n(Tappez 3) Supprimer un Eleve  \n(Tappez 4) Voir/Modifer les notes d'un eleve\n");
		scanf("%d", &choix);
		scanf("%*[^\n]");
		char Eleve[(Max_Eleve_name + 2)];
		if (choix == 0) { break; }
		else if (choix == 1) {
			printf("Entrez le Prenom.Nom  :\n");
			scanf("%"STR(Max_Eleve_name)"s", &Eleve);
			if (CheckInput(Eleve) < 0) { return; }
			scanf("%*[^\n]");
			if (checkEleve(Class, Eleve, 2) == 2) {
			}
			else {
				AddEleve(Class, Eleve, 1, NULL);
			}
		}
		else if (choix == 2) {
			printf("Entrez le Prenom.Nom :\n");
			scanf("%"STR(Max_Eleve_name)"s", &Eleve);
			if (CheckInput(Eleve) < 0) { return; }
			scanf("%*[^\n]");
			if (checkEleve(Class, Eleve, 0) != 0) {

			}
			else {
				char newEleve[(Max_Eleve_name + 2)];
				printf("Nouveau Prenom.Nom :\n");

				scanf("%"STR(Max_Eleve_name)"s", &newEleve);
				if (CheckInput(newEleve) < 0) { return; }
				scanf("%*[^\n]");
				if (checkEleve(Class, newEleve, 2) == 2) {
				} // regarde si le nouveau nom existe deja si oui return une erreur
				else {
					AddEleve(Class, Eleve, 2, newEleve);
				}
			}


		}
		else if (choix == 3) {
			printf("Indiquer le nom :\n");
			scanf("%"STR(Max_Eleve_name)"s", &Eleve);
			if (CheckInput(Eleve) < 0) { return; }
			scanf("%*[^\n]");
			DelEleve(Class, Eleve);

		}
		else if (choix == 4) {
			printf("Indiquez le nom de l'eleve :\n");
			scanf("%"STR(Max_Eleve_name)"s", &Eleve);
			if (CheckInput(Eleve) < 0) { return; }
			scanf("%*[^\n]");
			if (checkEleve(Class, Eleve, 0) != 0) {

			}
			else {
				ShowEleve(Class, Eleve);
			}

		}
		else {
			printf("\nVeuiller ressayer\n\n");
		}
	}
	return;
}
void AllEleve() {

	while (1 == 1) {
		char lstClass[110][(Max_class_name + 1)];
		int choix;
		int NbOfClass = getclasslst(&lstClass[0][0]);
		for (int i = 0; i < NbOfClass; i++) {
			printf("       --%s--", lstClass[i]);
			checkEleve(lstClass[i], NULL, 4);
			printf("\n");
		}
		choix = -1;
		printf("\n Que voulez vous faire :\n (Tappez 0) Pour Retourner en Arriere\n (Tappez 1) Pour Modifer un Eleve\n");
		scanf("%d", &choix);
		scanf("%*[^\n]");
		if (choix == 0) { break; }
		else if (choix == 1) {
			char Class[(Max_class_name + 1)];
			char Eleve[(Max_Eleve_name + 2)];
			printf("Quel est le nom de l'Eleve ? :\n");
			scanf("%"STR(Max_Eleve_name)"s", &Eleve);
			if (CheckInput(Eleve) < 0) { return; }
			scanf("%*[^\n]");
			printf("De quel CLass ? :\n");

			scanf("%"STR(Max_class_name)"s", &Class);
			if (CheckInput(Class) < 0) { return; }
			scanf("%*[^\n]");
			if (checkEleve(Class, Eleve, 0) != 0) {

			}
			else {
				ShowEleve(Class, Eleve);
			}
		}
		else {
			printf("\n     --Veuiller ressayez\n\n");
		}

	}
}



int getmatlst(char Class[(Max_class_name + 1)], char* lstMat) {

	char filename[Max_path_name] = "SCHOOL/";
	strcat(filename, Class);
	strcat(filename, "/");						//construit le path pour une CLass

	DIR* d;
	struct dirent* dir;
	d = opendir(filename);
	int NbOfMat = 0;
	char lstMat2[110][(Max_Matiere_name + 1)];
	if (d)
	{
		while ((dir = readdir(d)) != NULL) { // sauvegarde toute les matieres

			if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0) || (strcmp(dir->d_name, "...eleve.txt") == 0)) {// pour eviter de confondre  (".","..","...eleve.txt")   avec une  matiere
				continue;
			}
			else {
				strcpy(lstMat2[NbOfMat], dir->d_name);
				NbOfMat++;
			}

		}
		closedir(d);
	}

	if (NbOfMat < 1) { return -1; }

	for (int h = 0; h < NbOfMat; h++) {
		for (int u = 0; u < (Max_Matiere_name + 1); u++) {

			*lstMat = lstMat2[h][u];
			lstMat++;
		}
	}
	return NbOfMat;

}
float MoyenneMat(char Matiere[(Max_Matiere_name + 1)], char Class[(Max_class_name + 1)]) {

	char filename[Max_path_name] = "SCHOOL/";
	strcat(filename, Class);
	strcat(filename, "/");						//construit le path pour une matiere
	strcat(filename, Matiere);
	DIR* d;
	struct dirent* dir;
	d = opendir(filename);
	int i = 0;
	char lsteleve[110][(Max_Eleve_name + 2)];
	if (d)
	{
		i = 0;

		while ((dir = readdir(d)) != NULL)   // sauvegarde la liste des eleves dans un tableau
		{
			strcpy(lsteleve[i], dir->d_name);
			i++;

		}
		closedir(d);

	}

	float test;
	float sum = 0;
	float moyenne;
	int NbdNote = 0;
	for (int u = 2; u < i; u++) {  // boucle pour avoir toute les moyennes des eleves dans cette matiere
		test = getvalnote(Matiere, Class, lsteleve[u], 1);
		if (test >= 0) {
			sum += test;
			NbdNote++;
		}
	}
	if (NbdNote > 0) {
		moyenne = sum / NbdNote;
		return moyenne;  // retourne la moyenne pour la print ou l'utiliser dans une autre fonction
	}
	else {
		return -1;
	}
}
float MoyenneMatForAllClasses(char Matiere[(Max_Matiere_name + 1)]) {
	char lstClass[110][(Max_class_name + 1)];
	int NbOfClass = getclasslst(&lstClass[0][0]);
	float test = 0;
	float moyenne = 0;
	float NbOfMoy = 0;
	for (int i = 0; i < NbOfClass; i++) {
		if ((test = MoyenneMat(Matiere, lstClass[i])) >= 0) {
			moyenne += test;
			NbOfMoy++;
		}
	}
	if (NbOfMoy > 0) {
		moyenne = moyenne / NbOfMoy;
		return moyenne;
	}
	else { return -1; }


}
int showlstmat(char Class[(Max_class_name + 1)], int print) {


	char lstMat[110][(Max_Matiere_name + 1)];
	int NbOfMat = getmatlst(Class, &lstMat[0][0]);
	if (NbOfMat == -1) { return-1; }
	for (int i = 0; i < NbOfMat; i++) {

		if (print != 4) {
			printf("  --%s\n", lstMat[i]); // print la matiere
		}
		else {// print la matiere et la moyenne des eleves dans cette matiere
			printf("  --%s : ", lstMat[i]);
			float moy;
			if ((moy = MoyenneMat(lstMat[i], Class)) >= 0) {
				printf("%.2f/20\n", moy);
			}
			else {
				printf("--Aucunes Notes--\n");
			}
		}


	}
	printf("\n");
}
void ShowAllMoyenneOfMat(char Matiere[Max_Matiere_name + 1]) {
	char lstClass[110][Max_class_name + 1];
	int NbofClass = getclasslst(&lstClass[0][0]);
	char filename[Max_path_name] = "SCHOOL/";
	char path[Max_path_name] = "SCHOOL/";
	for (int i = 0; i < NbofClass; i++) {
		strcat(filename, lstClass[i]);
		strcat(filename, "/");
		strcat(filename, Matiere);
		DIR* d = opendir(filename);
		if (d) {
			closedir(d);
			for (int h = 0; h < Max_path_name; h++) {
				filename[h] = path[h];
			}
			strcat(filename, lstClass[i]);
			strcat(filename, "/...eleve.txt");  // ouvre la liste des eleves
			FILE* ftr = fopen(filename, "a+");
			char eleve[110][(Max_Eleve_name + 2)];
			int u = 0;
			printf("      --%s--\n", lstClass[i]);
			while (fgets(eleve[u], sizeof(eleve), ftr) != NULL) { // sauvegarde la liste des eleves
				eleve[u][(length(eleve[u]) - 1)] = '\0';

				printf("  --%s : ", eleve[u]);           // calcule la moyenne d'une matiere de chaque eleve en les montrant
				float test;
				if ((test = getvalnote(Matiere, lstClass[i], eleve[u], 1)) >= 0) {
					printf("%.2f/20\n", test);
				}
				else
				{
					printf("--Aucune note--\n");
				}
				u++;
			}
			fclose(ftr);

		}
		for (int h = 0; h < Max_path_name; h++) {
			filename[h] = path[h];
		}
	}
}
int checkMat(char Matiere[(Max_Matiere_name + 1)]) {
	char lstClass[110][(Max_class_name + 1)];
	int NbOfClass = getclasslst(&lstClass[0][0]);
	printf("\n");
	int Classfound = 0;
	for (int i = 0; i < NbOfClass; i++) { // boucle pour passer par toute les class
		char filename[Max_path_name] = "SCHOOL/";
		strcat(filename, lstClass[i]);
		strcat(filename, "/");
		strcat(filename, Matiere);
		DIR* d = opendir(filename);
		if (d) {
			Classfound = 1;   // check si la class est trouver
			closedir(d);
		}

	}
	return Classfound;
}
void showMat(char Matiere[(Max_Matiere_name + 1)]) {
	int choix = -1;
	while (choix != 0) {

		choix = -1;
		printf("\n            --Que voulez vous savoir a propos de la Matiere %s  ? \n(Tappez 0) Pour retourner en arriere \n(Tappez 1) Voir La moyenne Generale de la Matiere  \n(Tappez 2) Voir la Moyenne d'une Classe  \n(Tappez 3) Voir les Moyenne de toutes les Classe  \n", Matiere);
		scanf("%*[^\n]");
		scanf("%d", &choix);
		scanf("%*[^\n]");
		if (choix == 0) {}
		else if (choix == 1) {
			float test;
			if ((test = MoyenneMatForAllClasses(Matiere)) >= 0) {
				printf("\n   --%.2f--\n\n", test);
			}
			else { printf("\n   --Aucunes Notes--\n"); }  // print la moyenne de la Matiere mentionner seulement si la moyenne n'a pas retourner d'erreur

		}
		else if (choix == 2) {
			char Class[(Max_class_name + 1)];

			char lstClass[110][(Max_class_name + 1)];
			int NbOfClass = getclasslst(&lstClass[0][0]);
			printf("\n");
			for (int i = 0; i < NbOfClass; i++) { // boucle pour passer par toute les class
				char filename[Max_path_name] = "SCHOOL/";
				strcat(filename, lstClass[i]);
				strcat(filename, "/");
				strcat(filename, Matiere);
				DIR* d = opendir(filename);
				if (d) {
					printf("  --%s\n", lstClass[i]); // affiche toutes les Class
					closedir(d);
				}

			}
			printf("\nIndiquez le nom de la Class a voir :\n");
			scanf("%"STR(Max_class_name)"s", &Class);
			if (CheckInput(Class) < 0) { return; }
			scanf("%*[^\n]");
			float test;
			if ((test = MoyenneMat(Matiere, Class)) >= 0) {
				printf("%.2f", test);

			}
			else { printf("\n    --Aucunes Notes--\n"); }
		}
		else if (choix == 3) {
			ShowAllMoyenneOfMat(Matiere);
		}


		else {
			printf("\n\n        --Veuillez ressayez--\n\n");
			scanf("%*[^\n]");

		}

	}




}
int AddMat(char Matiere[(Max_Matiere_name + 1)], char Class[(Max_class_name + 1)]) {
	char filename[Max_path_name] = "SCHOOL/";
	char path[Max_path_name];
	strcat(filename, Class);
	strcat(filename, "/");
	for (int i = 0; i < Max_path_name; i++) {  // sauvegarde le path de la class pour le reutiliser apres un strcat
		path[i] = filename[i];
	}
	strcat(filename, Matiere);

	if (mkdir(filename)) { printf("\n   --Cette matiere existe deja\n"); return -1; }        // cree le repertoir de la matiere

	for (int i = 0; i < Max_path_name; i++) {  // sauvegarde le path de la class pour le reutiliser apres un strcat
		filename[i] = path[i];
	}

	strcat(filename, "...eleve.txt");         // cree le chemin vers la liste des eleve
	FILE* ftr = fopen(filename, "r");
	if (ftr != NULL) {
		char Eleve[110][(Max_Eleve_name + 2)];
		int Nbofeleve = 0;

		while (fgets(Eleve[Nbofeleve], sizeof(Eleve), ftr) != NULL) { // boucle pour save le string de tout les elves dans un tableau

			Eleve[Nbofeleve][(length(Eleve[Nbofeleve]) - 1)] = '\0'; // retire le '\n' de l'eleve

			Nbofeleve++;
		}
		fclose(ftr);


		for (int f = 0; f < Nbofeleve; f++) {  // boucle pour ajouter tous les eleves de la liste dans le fichier de la matiere
			for (int u = 0; u < Max_path_name; u++) {
				filename[u] = path[u];
			}
			strcat(filename, Matiere);
			strcat(filename, "/");
			strcat(filename, Eleve[f]);
			mkdir(filename);

		}
	}
	return 0;
}
int DelMat(char Matiere[(Max_Matiere_name + 1)], char Class[(Max_class_name + 1)], int print) {
	char filename[Max_path_name] = "SCHOOL/";
	char path[Max_path_name];
	char lstEleve[110][(Max_Eleve_name + 2)];
	strcat(filename, Class);
	strcat(filename, "/");
	strcat(filename, Matiere);
	strcat(filename, "/");
	for (int u = 0; u < Max_path_name; u++) { // sauvegarde le path de base pour le reutiliser apres un strcat
		path[u] = filename[u];
	}
	DIR* d;
	struct dirent* dir;
	d = opendir(filename);
	int i = 0;
	while ((dir = readdir(d)) != NULL) {//    boucle pour avoir la liste des eleves dans une matiere
		strcpy(lstEleve[i], dir->d_name);
		lstEleve[i][(length(lstEleve[i]))] = '\0';
		i++;
	}
	closedir(d);
	for (int h = 2; h < i; h++) {       // boucle pour passer par tout les eleve

		getvalnote(Matiere, Class, lstEleve[h], -3);              // suprimme toute les note de l'eleve
		rmdir(strcat(filename, lstEleve[h]));            // suprimme son repertoir
		for (int u = 0; u < Max_path_name; u++) {                // remet le path pour changer d'eleve
			filename[u] = path[u];
		}
	}

	int ret = rmdir(filename);          // retire le repertoir de la matiere

	if (ret == 0 && print == 1) {
		printf("\n    ---Repertoire supprimer---\n\n");
	}
	else if (ret != 0) {
		printf("\nOops cette Matiere n'existe pas!\n");
		scanf("%*[^\n]");
	}
	return 0;


}
int renameMat(char Class[(Max_class_name + 1)], char Matiere[(Max_Matiere_name + 1)], char newmat[(Max_Matiere_name + 1)], int test) {
	if (Matiere == NULL) { printf("ERREUUUR"); return -1; }

	char filename[Max_path_name] = "SCHOOL/";
	strcat(filename, Class);
	strcat(filename, "/");
	DIR* d = opendir(strcat(filename, Matiere));


	if (test == 1) {

		if (!d) { printf("\nOops cette Matiere n'existe pas!\n"); return -1; }
		closedir(d);
	}
	if (test == 2) {
		char filename2[Max_path_name] = "SCHOOL/"; // cree 2 path pour renommer la matier dans le path 1 
		strcat(filename2, Class);
		strcat(filename2, "/");
		DIR* d = opendir(strcat(filename2, newmat));
		if (d) { printf("\nOops cette Matiere existe deja !\n"); closedir(d); return -1; }
		rename(filename, filename2); // renomme la matiere 
	}

}
void modifyMat(char Class[(Max_class_name + 1)]) {

	while (1 == 1) {
		int mat = -1;
		printf("      --Matieres--\n");
		if (showlstmat(Class, 1) == -1) { printf("   --Aucune Matiere--\n"); }
		printf("(Tappez 0) Pour retourner en arriere \n(Tappez 1) Pour Ajouter une Matiere \n(Tappez 2) Pour Renommer une Matiere \n(Tappez 3) Pour Supprimer une Matiere\n");
		scanf("%d", &mat);
		scanf("%*[^\n]");
		if (mat == 0) { break; }
		else if (mat == 1) {
			printf("Indiquer le nom de la Matiere :\n");
			char Matiere[(Max_Matiere_name + 1)];
			scanf("%"STR(Max_Matiere_name)"s", &Matiere);
			if (CheckInput(Matiere) < 0) { return; }
			scanf("%*[^\n]");
			AddMat(Matiere, Class);
		}
		else if (mat == 2) {
			char Matiere[(Max_Matiere_name + 1)];
			char newmat[(Max_Matiere_name + 1)];
			printf("Indiquer le Nom de la Matier :\n");
			scanf("%"STR(Max_Matiere_name)"s", &Matiere);
			if (CheckInput(Matiere) < 0) { return; }
			scanf("%*[^\n]");
			if (renameMat(Class, Matiere, NULL, 1) != -1) {
				printf("Le nouveau Nom :\n");
				scanf("%"STR(Max_Matiere_name)"s", &newmat);
				if (CheckInput(newmat) < 0) { return; }
				scanf("%*[^\n]");
				renameMat(Class, Matiere, newmat, 2);
			}
			else {}
		}
		else if (mat == 3) {
			char matiere[(Max_Matiere_name + 1)];

			printf("\nIndiquez le nom de la Matiere :\n");
			scanf("%"STR(Max_Matiere_name)"s", &matiere);
			if (CheckInput(matiere) < 0) { return; }
			scanf("%*[^\n]");
			DelMat(matiere, Class, 1);


		}
	}
}
int AllMat() {
	char lstClass[110][(Max_class_name + 1)];
	int NbOfClasse = getclasslst(&lstClass[0][0]);
	if (NbOfClasse <= 0) { return -1; }
	for (int u = 0; u < NbOfClasse; u++) {

		printf("       --%s--\n", lstClass[u]);
		if (showlstmat(lstClass[u], 4) == -1) { printf("   --Aucune Matiere--\n"); }
	}

}
void MenuMatiere() {
	int choix;
	while (1 == 1) {
		choix = -1;
		AllMat();
		printf("\n (Tappez 0) Pour retourner en Arriere\n (Tappez 1) voir les Info d'une Matiere\n");
		scanf("%d", &choix);
		scanf("%*[^\n]");
		if (choix == 0) { break; }

		else if (choix == 1) {
			char Matiere[(Max_Matiere_name + 1)];
			printf("Indiquez La Matiere :\n");
			scanf("%"STR(Max_Matiere_name)"s", &Matiere);
			if (CheckInput(Matiere) < 0) { return; }
			if (checkMat(Matiere) == 1) {
				showMat(Matiere);
			}
			else { printf("\n   --Cette Matiere n'existe pas !--\n\n"); }
		}
		else {
			printf("\n  --Veuillez ressayer--\n\n");

		}


	}
}


int getclasslst(char* lstClass) {

	char filename[Max_path_name] = "SCHOOL/";
	DIR* d;
	struct dirent* dir;
	d = opendir(filename);  // ouvre le dossier SCHOOL/

	char lstClass2[110][(Max_class_name + 1)];
	int NbOfClass = 0;
	if (d)
	{


		while ((dir = readdir(d)) != NULL) // boucle pour passer par toute les class
		{
			if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0)) {
				continue;
			}
			else {
				strcpy(lstClass2[NbOfClass], dir->d_name);
				NbOfClass++;
			}



		}
		closedir(d);
		if (NbOfClass < 1) { printf("\n    --Aucune Class--\n"); return -1; }
	}

	for (int h = 0; h < NbOfClass; h++) {
		for (int u = 0; u < (Max_class_name + 1); u++) {
			*lstClass = lstClass2[h][u];
			lstClass++;


		}

	}
	return NbOfClass;
}
float MoyenneClass(char Class[(Max_class_name + 1)]) {
	char filename[Max_path_name] = "SCHOOL/";
	char eleve[110][(Max_Eleve_name + 2)];
	strcat(filename, Class);
	strcat(filename, "/");
	strcat(filename, "...eleve.txt"); // cree le chemin vers la liste des eleves
	FILE* ftr = fopen(filename, "r");
	if (ftr == NULL) { return -1; }
	int i = 0;
	while (fgets(eleve[i], sizeof(eleve[i]), ftr) != NULL) { // boucle pour avoir tout les eleve de laa liste
		eleve[i][strcspn(eleve[i], "\n")] = '\0';
		i++;
	}
	fclose(ftr);
	float sum = 0;
	float test = 0;
	float moyenne = 0;
	float Nbneleve = 0;

	for (int u = 0; u < i; u++) { // boucle pour calculer la moyenne des moyenne generale des eleves
		test = 0.000000;
		test = moyenneGofEleve(Class, eleve[u], 4);
		if (test >= 0) {
			sum += test;
			Nbneleve++;
		}
	}
	if (Nbneleve == 0) { return -1; }
	moyenne = sum / Nbneleve;
	return moyenne; // return la moyenne generale des eleves pour les print ou les utiliser dans une fonction
}
void RenameClass() {
	char Class[(Max_class_name + 1)];
	char newClass[(Max_class_name + 1)];
	char filename[Max_path_name] = "SCHOOL/";
	char newfilename[Max_path_name] = "SCHOOL/";
	printf("Indiquer le nom de la Class :\n");
	scanf("%"STR(Max_class_name)"s", &Class);
	if (CheckInput(Class) < 0) { return; }
	scanf("%*[^\n]");
	strcat(filename, Class);
	DIR* d = opendir(filename);
	if (!d) { printf("\n  --Cette Class n'existe pas !--\n\n"); return; }
	closedir(d);
	printf("Le nouveau nom :\n");
	scanf("%"STR(Max_class_name)"s", &newClass);
	if (CheckInput(newClass) < 0) { return; }
	scanf("%*[^\n]");
	strcat(newfilename, newClass);
	d = opendir(newfilename);
	if (d) { printf("\n  --Cette Class existe deja !--\n\n"); closedir(d); return; }

	rename(filename, newfilename);   // renomme l'acienne class par la nouvelle


}
void AddClass() {
	char Class[(Max_class_name + 1)];
	char filename[Max_path_name] = "SCHOOL/";
	printf("Indiquer le nom de la Class :\n");
	scanf("%"STR(Max_class_name)"s", &Class);
	if (CheckInput(Class) < 0) { return; }
	scanf("%*[^\n]");
	strcat(filename, Class);
	DIR* d = opendir(filename);
	if (d) { printf("\n  --Cette Class existe deja !\n\n"); closedir(d); }
	mkdir(filename);         // fait un nouveau repertoir pour la nouvelle class
}
void DelClass(char Class[(Max_class_name + 1)]) {

	char filename[Max_path_name] = "SCHOOL/";
	strcat(filename, Class);
	DIR* d = opendir(filename);
	if (!d) { printf("\n  --Cette Class n'existe pas !--\n\n"); return; }
	closedir(d);
	strcat(filename, "/...eleve.txt");                 //cree le chemin vers le fichier de la liste des eleves
	char lsteleve[110][(Max_Eleve_name + 2)];

	FILE* ftr = fopen(filename, "r");
	int i = 0;
	if (ftr != NULL) {
		while (fgets(lsteleve[i], sizeof(lsteleve), ftr) != NULL) { //boucle pour avoir la liste des eleves
			lsteleve[i][(length(lsteleve[i]) - 1)] = '\0';
			i++;
		}
		fclose(ftr);
		remove(filename);              //supprime le fichier de la liste des eleve
		for (int u = 0; u < i; u++) {                                //boucle pour supprimer tout les eleve
			DelEleve(Class, lsteleve[u]);

		}
	}

	char filename2[Max_path_name] = "SCHOOL/";
	d = opendir(strcat(filename2, Class));              //ouvre le fichier de la class
	struct dirent* dir;
	char LstMat[110][(Max_Matiere_name + 1)];
	i = 0;
	while ((dir = readdir(d)) != NULL) {          // boucle pour avoir la liste des Matiere

		strcpy(LstMat[i], dir->d_name);
		i++;
	}
	closedir(d);

	for (int u = 3; u < i; u++) {     // fait une boucle pour supprimer les Matiere
		DelMat(LstMat[u], Class, 0);
	}
	char filename3[Max_path_name] = "SCHOOL/";
	rmdir(strcat(filename3, Class));             //suprime repertoir de la class
}
void Classes() {
	int choix = -1;
	while (choix != 0) {
		choix = -1;
		char lstClass[110][(Max_class_name + 1)];
		int NbOfClass = getclasslst(&lstClass[0][0]);
		printf("\n");
		for (int i = 0; i < NbOfClass; i++) { // boucle pour passer par toute les class
			printf("  --%s\n", lstClass[i]); // affiche toutes les Class
		} // affiche toute les classes
		printf("(Tappez 0) Pour retourner en arriere \n(Tappez 1) Pour Ajouter une Class \n(Tappez 2) Pour Renommer une Class \n(Tappez 3) Pour Supprimer une Class\n");
		scanf("%d", &choix);
		scanf("%*[^\n]");
		if (choix == 0) {}
		if (choix == 1) {
			AddClass();

		}
		if (choix == 2) {
			RenameClass();
		}
		if (choix == 3) {
			char Class[(Max_class_name + 1)];
			printf("Indiquer le nom de la Class :\n");
			scanf("%"STR(Max_class_name)"s", &Class);
			if (CheckInput(Class) < 0) { return; }
			scanf("%*[^\n]");

			DelClass(Class);

		}
	}
}
void infoclass() {
	printf("Quel CLass voulez vous voir? :\n");
	char Class[(Max_class_name + 1)];
	char c;
	scanf("%"STR(Max_class_name)"s", &Class);
	if (CheckInput(Class) < 0) { return; }
	scanf("%*[^\n]");

	int choix = -1;
	while (choix != 0) {
		choix = -1;
		if (checkEleve(Class, NULL, 0) == -1) { break; } // verifie si la class existe 
		printf("      --Matieres--\n");
		if (showlstmat(Class, 4) == -1) { printf("   --Aucune Matiere--\n"); }// print les matieres et leur moyenne 
		printf("      --Eleves--");
		checkEleve(Class, NULL, 4);// print les eleves et leur moyenne
		printf("\n(Tappez 0) Pour retourner en arriere \n(Tappez 1) Modifier des Eleves  \n(Tappez 2) Modifier des Matieres  \n(Tappez 3) Voir la moyenne Generale des Eleves \n");
		scanf("%d", &choix);
		scanf("%*[^\n]");
		if (choix == 0) {}
		else if (choix == 1) {
			modifEleve(Class);
		}
		else if (choix == 2) {
			modifyMat(Class);
		}
		else if (choix == 3) {
			float test = MoyenneClass(Class);
			if (test >= 0) {
				printf("\n     --%.2f--\n", test);
			}
			else { printf("  --Aucune Notes--\n"); }
		}
		else {
			printf("\n\n        --Veuillez ressayez--\n\n");

		}
	}
}
void modifClass() {

	while (1 == 1) {
		int choix = -1;

		printf("\n      -- SCHOOL --\n");
		char lstClass[110][(Max_class_name + 1)];
		int NbOfClass = getclasslst(&lstClass[0][0]);
		for (int i = 0; i < NbOfClass; i++) { // boucle pour passer par toute les class
			printf("  --%s  :  ", lstClass[i]); // affiche toutes les Class
			float test = MoyenneClass(lstClass[i]);
			if (test >= 0) {
				printf(" %.2f/20\n", test);
			}
			else { printf(" --Aucune Notes--\n"); }

		}
		printf("\n(Tappez 0) Pour retourner en arriere \n(Tappez 1) Voir/Modifier les Info d'une Class  \n(Tappez 2) Modifier les Class  \n(Tappez 3) Voir la moyenne Generale de tout les eleves  \n");
		scanf("%d", &choix);
		scanf("%*[^\n]");
		if (choix == 0) { break; }
		if (choix == 1) {
			infoclass();
		}
		else if (choix == 2) {
			Classes();
		}
		else if (choix == 3) {
			DIR* d = opendir("SCHOOL/");
			struct dirent* dir;
			float moyenne = 0;
			float nbofclass = 0;
			float test;
			while ((dir = readdir(d)) != NULL) {
				test = MoyenneClass((dir->d_name));
				if (test >= 0) {
					moyenne += test;
					nbofclass++;
				}
			}
			if (nbofclass > 0) { printf("\n     --%.2f/20--\n", (moyenne = (moyenne / nbofclass))); }
			closedir(d);
		}
		else {

		}
	}
	return;
}


int main() {
	mkdir("SCHOOL");
	while (1 == 1) {

		int choice = -1;
		printf("                              --Que Voulez vous Faire ? --\n Voir/Modifier les Class (Tappez 1) || Voir/Modifier les Matiere (Tappez 2) || Voir/Modifier Les Notes des Eleves (Tappez 3)\n");
		scanf("%d", &choice);
		scanf("%*[^\n]");

		if (choice == 1) {

			modifClass();
		}

		else if (choice == 2) {
			MenuMatiere();

		}
		else if (choice == 3) {
			AllEleve();
		}
		else {
			printf("\n\n        --Veuillez ressayez--\n\n");
		}
	}
	return 0;
}