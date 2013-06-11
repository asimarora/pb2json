CPPFLAGS = -I.. -I.

all:person.pb.cc test_person test_json json2json
person.pb.cc:person.proto
	protoc --cpp_out=./ person.proto
test_person:test_person.cpp person.pb.cc person.pb.h
	g++ -g -o test_person test_person.cpp person.pb.cc -lprotobuf
test_json:test_json.cpp person.pb.cc person.pb.h 
	g++ -g -o test_json test_json.cpp person.pb.cc ../libpb2json.a  -I../  -lprotobuf -ljansson

json2json: json2json.o person.pb.o ../libpb2json.a
	$(CXX) -o $@ $^ -lprotobuf -ljansson


clean:
	rm -f test_person test_json person.pb.cc person.pb.h dump
