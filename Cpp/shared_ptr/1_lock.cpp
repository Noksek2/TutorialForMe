#include <iostream>

int main() {
	std::shared_ptr<int>si = std::make_shared<int>(100);
	std::weak_ptr<int> wi = si;

	while (true) {
		auto pwi = wi.lock();
		if (pwi == nullptr) {
			std::cout << "wow garbage\n";
			break;
		}
		else{
			std::cout << *pwi<<" yeah\n";
			si = nullptr;
		}
	}
	return 0;
}
