#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>

using namespace std;


namespace {
	
	class Echo
	{
	public:
	   Echo(): _x(new string("abcd")) {
	   }
	   virtual ~Echo() {
           cout << "C'est la fin" << endl;
           delete _x;
	   }

        string* _x;
	};
	
    Echo echo;

}

extern "C" void* routine(void*) {
    while (true) {
        cout << "Detache, " << *(echo._x) << endl;
    }
    cout << "Fin du thread" << endl;
    
    return NULL;
}


int main() {
    pthread_t      tid;  // thread ID
    pthread_attr_t attr; // thread attribute

    // set thread detachstate attribute to DETACHED 
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // create the thread 
    pthread_create(&tid, &attr, routine, NULL);
    
    sleep(1);
    
    cout << "Fin" << endl;
    return 0;
}
