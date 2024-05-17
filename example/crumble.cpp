#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

class Crumble {
public:

	void buy(const char * what, int quantity, const char * unit) {
		cout << "buy " << quantity << " " << unit << " of " << what << endl;
		sleep(1);
	}

	void buy_stuff(void) {
		buy("sugar", 125, "grams");
		buy("butter", 125, "grams");
		buy("wheat", 200, "grams");
		buy("salt", 1, "pinch");
		buy("apple", 5, "pieces");
		sleep(1);
	}

	void skin_and_dice() {
		cout << "skin apples and make little dices" << endl;
		sleep(1);
	}

	int prepare_apples() {
		skin_and_dice();
		sleep(1);
		return 1;
	}

	int mix(const char * a, const char * b, const char * c, const char * d) {
		cout << "mix " << a << " with " << b << ", " << c << " and " << d << endl;
		sleep(1);
		return 1;
	}

	int put(char * what, char * where) {
		cout << "put " << what << " " << where << endl;
		sleep(1);
	}

	void finalize(int apples, int crumble) {
		put("apples", "below");
		put("crumble", "on top");
		sleep(1);
	}

	void bake(const char * what, int temperature_deg, int time_min) {
		cout << "cook " << what << " at " << temperature_deg
				<< " degrees for " << time_min << " minutes" << endl;
		sleep(1);
	}

	void cook() {
		put("apple crumble", "in oven");
		bake("apple crumble", 220, 45);
		sleep(1);
	}

	void make_apple_crumble() {
		int	apples;
		int	crumble;

		buy_stuff();
		apples = prepare_apples();
		crumble = mix("butter", "sugar", "wheat", "salt");
		finalize(apples, crumble);
		cook();
		sleep(1);
	}

};

int main(int argc, char * argv[]) {
	Crumble crumble;
	crumble.make_apple_crumble();
	return 0;
}
