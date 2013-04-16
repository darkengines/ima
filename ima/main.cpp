#include "System.h"

int main(int argc, char** argv) {
	System* system = new System();
	system->Initialize();
	system->Run();
	system->Shutdown();
	return 0;
}