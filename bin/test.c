#include <stdio.h>
#include <alsa/asoundlib.h>

int main() {

    int icard = -1;
	for (;;) {
		snd_card_next(&icard);
		if (icard == -1)
			break;
		...
	}

    return 0;
}

