#include <stdio.h>
#include <string.h>

int main (void) {
	char words[2][40] = { "jd1.png", "jd2.png" };
	printf("%s", words[0]);
	char path[40] = "data/images/";
	printf("%s", path);
	strcat(path, words[0]);
	printf("%s", path);
}
