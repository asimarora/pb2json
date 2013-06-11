#include "pb2json.h"

using std::string;
string hex_encode(const string& input)
{
	static const char* const lut = "0123456789abcdef";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const unsigned char c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}
	return output;
}
string hex_decode(const string& input)
{
	static const char* const lut = ""
"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" /* 0x00 */ "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" // 0x10
"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" /* 0x20 */ "\0\1\2\3\4\5\6\7\x08\x09\0\0\0\0\0\0" // 0x30
"\0\x0a\x0b\x0c\x0d\x0e\x0f\0\0\0\0\0\0\0\0\0" /* 0x40 */ "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" // 0x50
"\0\x0a\x0b\x0c\x0d\x0e\x0f\0\0\0\0\0\0\0\0\0" /* 0x60 */ "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" // 0x70
"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" /* 0x80 */ "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" // 0x90
"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" /* 0xa0 */ "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" // 0xb0
"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" /* 0xc0 */ "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" // 0xd0
"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" /* 0xe0 */ "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" // 0xf0
"";
	size_t len = input.length();

	std::string output;
	output.reserve(len/2);
	for (size_t i = 0; i + 1 < len; i += 2)
	{
		char h = lut[(unsigned char) input[i]];
		char l = lut[(unsigned char) input[i+1]];
		output.push_back((h << 4) | l);
	}
	return output;
}
char * pb2json(const Message &msg)
{
	json_t *root = parse_msg(&msg);
	char *json = json_dumps(root,0);
	json_decref(root);
	return json; // should be freed by caller
}
char * pb2json( Message *msg,const char *buf ,int len)
{
	string s (buf,len);
	msg->ParseFromString(s);
	json_t *root = parse_msg(msg);
	char *json = json_dumps(root,0);
	json_decref(root);
	return json; // should be freed by caller
}
static json_t *parse_repeated_field(const Message *msg,const Reflection * ref,const FieldDescriptor *field)
{
	size_t count = ref->FieldSize(*msg,field);
	json_t *arr = json_array();	
	if(!arr)return NULL;
	switch(field->cpp_type())	
	{
		case FieldDescriptor::CPPTYPE_DOUBLE:
			for(size_t i = 0 ; i != count ; ++i)
			{
				double	value1 = ref->GetRepeatedDouble(*msg,field,i);
				json_array_append_new(arr,json_real(value1));	
			}
			break;
		case FieldDescriptor::CPPTYPE_FLOAT:
			for(size_t i = 0 ; i != count ; ++i)
			{
				float value2 = ref->GetRepeatedFloat(*msg,field,i);
				json_array_append_new(arr,json_real(value2));	
			}
			break;
		case FieldDescriptor::CPPTYPE_INT64:
			for(size_t i = 0 ; i != count ; ++i)
			{
				int64_t value3 = ref->GetRepeatedInt64(*msg,field,i);
				json_array_append_new(arr,json_integer(value3))	;
			}
			break;
		case FieldDescriptor::CPPTYPE_UINT64:
			for(size_t i = 0 ; i != count ; ++i)
			{
				uint64_t value4 = ref->GetRepeatedUInt64(*msg,field,i);
				json_array_append_new(arr,json_integer(value4));	
			}
			break;
		case FieldDescriptor::CPPTYPE_INT32:
			for(size_t i = 0 ; i != count ; ++i)
			{
				int32_t value5 = ref->GetRepeatedInt32(*msg,field,i);
				json_array_append_new(arr,json_integer(value5));	
			}
			break;
		case FieldDescriptor::CPPTYPE_UINT32:
			for(size_t i = 0 ; i != count ; ++i)
			{
				uint32_t value6 = ref->GetRepeatedUInt32(*msg,field,i);
				json_array_append_new(arr,json_integer(value6));	
			}
			break;
		case FieldDescriptor::CPPTYPE_BOOL:
			for(size_t i = 0 ; i != count ; ++i)
			{
				bool value7 = ref->GetRepeatedBool(*msg,field,i);
				json_array_append_new(arr,value7?json_true():json_false())	;
			}
			break;
		case FieldDescriptor::CPPTYPE_STRING:
			for(size_t i = 0 ; i != count ; ++i)
			{
				string value8 = ref->GetRepeatedString(*msg,field,i);
				json_array_append_new(arr,json_string(value8.c_str()));	
			}
			break;
		case FieldDescriptor::CPPTYPE_MESSAGE:
			for(size_t i = 0 ; i != count ; ++i)
			{
				const Message *value9 = &(ref->GetRepeatedMessage(*msg,field,i));
				json_array_append_new(arr,parse_msg(value9));
			}
			break;
		case FieldDescriptor::CPPTYPE_ENUM:
			for(size_t i = 0 ; i != count ; ++i)
			{
				const EnumValueDescriptor* value10 = ref->GetRepeatedEnum(*msg,field,i);
				json_array_append_new(arr,json_integer(value10->number()));	
			}
			break;
		default:
			break;
	}
	return arr;
}
static json_t *parse_msg(const Message *msg)
{
	const Descriptor *d = msg->GetDescriptor();
	if(!d)return NULL;
	size_t count = d->field_count();
	json_t *root = json_object();
	if(!root)return NULL;
	for (size_t i = 0; i != count ; ++i)
	{
		const FieldDescriptor *field = d->field(i);
		if(!field)return NULL;

		const Reflection *ref = msg->GetReflection();
		if(!ref)return NULL;
		const char *name = field->name().c_str();
		if(field->is_repeated())
			json_object_set_new(root,name,parse_repeated_field(msg,ref,field));
		if(!field->is_repeated() && ref->HasField(*msg,field))
		{

			double value1;
			float value2;
			int64_t value3;
			uint64_t value4;
			int32_t value5;
			uint32_t value6;
			bool value7;
			string value8;
			const Message *value9;
			const EnumValueDescriptor *value10;

			switch (field->cpp_type())
			{
				case FieldDescriptor::CPPTYPE_DOUBLE:
					value1 = ref->GetDouble(*msg,field);
					json_object_set_new(root,name,json_real(value1));	
					break;
				case FieldDescriptor::CPPTYPE_FLOAT:
					value2 = ref->GetFloat(*msg,field);
					json_object_set_new(root,name,json_real(value2));	
					break;
				case FieldDescriptor::CPPTYPE_INT64:
					value3 = ref->GetInt64(*msg,field);
					json_object_set_new(root,name,json_integer(value3))	;
					break;
				case FieldDescriptor::CPPTYPE_UINT64:
					value4 = ref->GetUInt64(*msg,field);
					json_object_set_new(root,name,json_integer(value4));	
					break;
				case FieldDescriptor::CPPTYPE_INT32:
					value5 = ref->GetInt32(*msg,field);
					json_object_set_new(root,name,json_integer(value5));	
					break;
				case FieldDescriptor::CPPTYPE_UINT32:
					value6 = ref->GetUInt32(*msg,field);
					json_object_set_new(root,name,json_integer(value6));	
					break;
				case FieldDescriptor::CPPTYPE_BOOL:
					value7 = ref->GetBool(*msg,field);
					json_object_set_new(root,name,value7?json_true():json_false())	;
					break;
				case FieldDescriptor::CPPTYPE_STRING:
					if (field->type() == FieldDescriptor::TYPE_BYTES) {
						value8 = hex_encode(ref->GetString(*msg,field));
					} else {
						value8 = ref->GetString(*msg,field);
					}
					json_object_set_new(root,name,json_string(value8.c_str()));	
					break;
				case FieldDescriptor::CPPTYPE_MESSAGE:
					value9 = &(ref->GetMessage(*msg,field));
					json_object_set_new(root,name,parse_msg(value9));
					break;//FIXME : parse Message
				case FieldDescriptor::CPPTYPE_ENUM:
					value10 = ref->GetEnum(*msg,field);
					json_object_set_new(root,name,json_integer(value10->number()));	
					break;
				default:
					break;
			}

		}

	}
	return root;

}

static int unparse_msg(Message *msg, const json_t *root, json_error_t *error)
{
	const Descriptor *d = msg->GetDescriptor();
	if(!d)return -1;
	size_t count = d->field_count();
	if(!root)return -1;
	for (size_t i = 0; i != count ; ++i)
	{
		const FieldDescriptor *field = d->field(i);
		if(!field)return -1;

		const Reflection *ref = msg->GetReflection();
		if(!ref)return -1;
		const char *name = field->name().c_str();
		if(field->is_repeated()) {
			//json_object_set_new(root,name,parse_repeated_field(msg,ref,field));
			continue;
		}
		json_t *jf = json_object_get(root, name);
		if (!jf) {
			if (field->is_required()) {
				printf("Field %s is required but missing\n", name);
				return -1;
			}
			continue;
		}

		switch (field->cpp_type())
		{
#define _PARSE(type, ctype, fmt, func) \
			case FieldDescriptor::type: {					\
				ctype value;						\
				int r = json_unpack_ex(jf, error, JSON_STRICT, fmt, &value); \
				if (r) return r;					\
				ref->func(msg, field, value);				\
				break;							\
			}

			_PARSE(CPPTYPE_DOUBLE, double, "f", SetDouble);
			_PARSE(CPPTYPE_FLOAT, double, "f", SetFloat);
			_PARSE(CPPTYPE_INT64, json_int_t, "I", SetInt64);
			_PARSE(CPPTYPE_UINT64, json_int_t, "I", SetUInt64);
			_PARSE(CPPTYPE_INT32, json_int_t, "I", SetInt32);
			_PARSE(CPPTYPE_UINT32, json_int_t, "I", SetUInt32);
			_PARSE(CPPTYPE_BOOL, int, "b", SetBool);

			case FieldDescriptor::CPPTYPE_STRING: {
				if (!json_is_string(jf))
					return -1;
				const char * value = json_string_value(jf);
				if (field->type() == FieldDescriptor::TYPE_BYTES) {
					ref->SetString(msg, field, hex_decode(value));
				} else {
					ref->SetString(msg, field, value);
				}
				break;
			}
			case FieldDescriptor::CPPTYPE_MESSAGE: {
				Message *mf = ref->MutableMessage(msg, field);
				if (unparse_msg(mf, jf, error))
					return -1;
				break;
			}
			case FieldDescriptor::CPPTYPE_ENUM: {
				if (!json_is_integer(jf))
					return -1;
				int value = json_integer_value(jf);
				const EnumValueDescriptor *ev = field->enum_type()->FindValueByNumber(value);
				if (!ev)
					return -1;
				ref->SetEnum(msg, field, ev);
				break;
			}
			default:
				break;
		}

	}
	return 0;
}

#include <errno.h>

int json2pb(Message *msg, const char *buf, size_t size)
{
	json_t *root;
	json_error_t error;

	root = json_loadb(buf, size, 0, &error);

	if (!root) {
		printf("Load failed: %s\n", error.text);
		return EINVAL;
	}

	struct _decref {
		json_t * j;
		_decref(json_t *json) : j(json) {}
		~_decref() { if (j) json_decref(j); }
	} _decref(root);

	if (!json_is_object(root))
		return EINVAL;

	return unparse_msg(msg, root, &error);
}
