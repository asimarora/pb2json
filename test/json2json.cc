#include "pb2json.h"
#include "person.pb.h"

const char json[] = "{\"name\":\"b\", \"id\":1, \"email\":\"0a0a0a\"}";

int main()
{
	Message *p = new Person();
	int r = json2pb(p, json, sizeof(json)-1);
	printf("Result: %d\n", r);
	printf("JSON: %s\n", pb2json(*p));
}
