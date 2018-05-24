int main(int argc, char *argv[]){

	system("pdflatex slides.tex");
	system("evince slides.pdf");
	system("rm *.aux *.log *.nav *.out *.snm *.toc *.vrb");

	return 0;
}
