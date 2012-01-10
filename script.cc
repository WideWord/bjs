#include "script.h"
#include "core.h"
#include <v8.h>

char* readFile(const char* file_name);

using namespace v8;
using namespace gfx;
using namespace input;
using namespace bjs;

Handle<ObjectTemplate> api;
Core* core = NULL;



Handle<Value> jsPrint(const Arguments& args) {/*int width, int height, bool full*/
    HandleScope handle_scope;

    if (args.Length() < 1) return Undefined();

    String::AsciiValue ascii(args[0]);

    printf("%s", *ascii);

    return Undefined();
}


Handle<Value> jsInit(const Arguments& args) {/*int width, int height, bool full*/
    if (args.Length() < 3) return False();
    HandleScope scope;

    if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsBoolean()) {
        return False();
    }

    core = new Core(args[0]->Int32Value(), args[1]->Int32Value(), args[2]->BooleanValue());

    return True();
}

Handle<Value> jsRun(const Arguments& args) {/*int width, int height, bool full*/
    HandleScope scope;
    return core->run()?True():False();
}

Handle<Value> jsExit(const Arguments& args) {/*int width, int height, bool full*/
    HandleScope scope;
    core->exit();
    return Undefined();
}

Handle<Value> jsRenderer_render(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    gfx::Renderer* r = (gfx::Renderer*)wrap->Value();
    r->render();
    return Undefined();
}

Handle<Value> jsRenderer_setScene(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    gfx::Renderer* r = (gfx::Renderer*)wrap->Value();
    if (args.Length() < 1)return Undefined();
    gfx::Scene* s = (gfx::Scene*)(Local<External>::Cast(args[0]->ToObject()->GetInternalField(0))->Value());
    r->setScene(s);
    return Undefined();
}

Handle<Value> jsGetRenderer(const Arguments& args) {
    HandleScope scope;
    Handle<ObjectTemplate> obj_templ = ObjectTemplate::New();
    obj_templ->SetInternalFieldCount(1);
    obj_templ->Set(String::New("render"), FunctionTemplate::New(jsRenderer_render));
    obj_templ->Set(String::New("setScene"), FunctionTemplate::New(jsRenderer_setScene));
    Handle<Object> res = obj_templ->NewInstance();
    res->SetInternalField(0, External::New(core->getRenderer()));
    return res;
}


Handle<Value> jsInputDevice_getKey(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    InputDevice* d = (InputDevice*)wrap->Value();
    if (args.Length() < 1)return Undefined();
    if (!args[0]->IsNumber())return Undefined();
    return d->getKey(args[0]->Int32Value())?True():False();
}

Handle<Value> jsInputDevice_getKeyDown(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    InputDevice* d = (InputDevice*)wrap->Value();
    if (args.Length() < 1)return Undefined();
    if (!args[0]->IsNumber())return Undefined();
    return d->getKeyDown(args[0]->Int32Value())?True():False();
}

Handle<Value> jsInputDevice_getKeyUp(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    InputDevice* d = (InputDevice*)wrap->Value();
    if (args.Length() < 1)return Undefined();
    if (!args[0]->IsNumber())return Undefined();
    return d->getKeyUp(args[0]->Int32Value())?True():False();
}

Handle<Value> jsGetInputDevice(const Arguments& args) {
    HandleScope scope;
    Handle<ObjectTemplate> obj_templ = ObjectTemplate::New();
    obj_templ->SetInternalFieldCount(1);
    obj_templ->Set(String::New("getKey"), FunctionTemplate::New(jsInputDevice_getKey));
    obj_templ->Set(String::New("getKeyDown"), FunctionTemplate::New(jsInputDevice_getKeyDown));
    obj_templ->Set(String::New("getKeyUp"), FunctionTemplate::New(jsInputDevice_getKeyUp));
    Handle<Object> res = obj_templ->NewInstance();
    res->SetInternalField(0, External::New(core->getInputDevice()));
    return res;
}

Handle<Value> jsScene_addSprite(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Scene* scn = (Scene*)wrap->Value();
    if (args.Length() < 1)return Undefined();
    Sprite* sp = (Sprite*)(Local<External>::Cast(args[0]->ToObject()->GetInternalField(0))->Value());
    scn->addSprite(sp);
    return Undefined();
}

Handle<Value> jsScene_free(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Scene* scn = (Scene*)wrap->Value();
    delete scn;
    return Undefined();
}


Handle<Value> jsNewScene(const Arguments& args) {
    HandleScope scope;
    Handle<ObjectTemplate> obj_templ = ObjectTemplate::New();
    obj_templ->SetInternalFieldCount(1);
    obj_templ->Set(String::New("addSprite"), FunctionTemplate::New(jsScene_addSprite));
    obj_templ->Set(String::New("free"), FunctionTemplate::New(jsScene_free));
    Handle<Object> res = obj_templ->NewInstance();
    Scene* scn = new Scene();
    res->SetInternalField(0, External::New(scn));
    return res;
}


Handle<Value> jsSprite_addFrame(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Sprite* spr = (Sprite*)wrap->Value();
    if (args.Length() < 1)return Undefined();
    Image* img = (Image*)(Local<External>::Cast(args[0]->ToObject()->GetInternalField(0))->Value());
    spr->addFrame(img);
    return Undefined();
}

Handle<Value> jsSprite_move(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Sprite* spr = (Sprite*)wrap->Value();
    if (args.Length() < 2)return Undefined();
    if (!args[0]->IsNumber() || !args[1]->IsNumber())return Undefined();
    spr->move(args[0]->Int32Value(), args[1]->Int32Value());
    return Undefined();
}

Handle<Value> jsSprite_show(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Sprite* spr = (Sprite*)wrap->Value();
    spr->show();
    return Undefined();
}

Handle<Value> jsSprite_hide(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Sprite* spr = (Sprite*)wrap->Value();
    spr->hide();
    return Undefined();
}

Handle<Value> jsSprite_setSpeed(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Sprite* spr = (Sprite*)wrap->Value();
    if (args.Length() < 1)return Undefined();
    if (!args[0]->IsNumber())return Undefined();
    spr->setSpeed(args[0]->Int32Value());
    return Undefined();
}

Handle<Value> jsNewSprite(const Arguments& args) {
    HandleScope scope;
    Handle<ObjectTemplate> obj_templ = ObjectTemplate::New();
    obj_templ->SetInternalFieldCount(1);
    obj_templ->Set(String::New("addFrame"), FunctionTemplate::New(jsSprite_addFrame));
    obj_templ->Set(String::New("move"), FunctionTemplate::New(jsSprite_move));
    obj_templ->Set(String::New("show"), FunctionTemplate::New(jsSprite_show));
    obj_templ->Set(String::New("hide"), FunctionTemplate::New(jsSprite_hide));
    obj_templ->Set(String::New("setSpeed"), FunctionTemplate::New(jsSprite_setSpeed));
    Handle<Object> res = obj_templ->NewInstance();

    if (args.Length() < 1)return Undefined();
    Image* img = (Image*)(Local<External>::Cast(args[0]->ToObject()->GetInternalField(0))->Value());

    Sprite* spr = new Sprite(img);
    res->SetInternalField(0, External::New(spr));
    return res;
}

Handle<Value> jsImage_free(const Arguments& args) {
    Local<Object> self = args.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    Image* img = (Image*)wrap->Value();
    delete img;
    return Undefined();
}

Handle<Value> jsNewImage(const Arguments& args) {
    HandleScope scope;
    Handle<ObjectTemplate> obj_templ = ObjectTemplate::New();
    obj_templ->SetInternalFieldCount(1);
    obj_templ->Set(String::New("free"), FunctionTemplate::New(jsImage_free));


    Handle<Object> res = obj_templ->NewInstance();

    if (args.Length() < 1)return Undefined();
    String::AsciiValue ascii(args[0]);

    Image* img = new Image(*ascii);

    res->SetInternalField(0, External::New(img));
    return res;
}



Handle<Value> jsInclude(const Arguments& args) {/*int width, int height, bool full*/
    HandleScope handle_scope;

    if (args.Length() < 1) return Undefined();

    String::AsciiValue ascii(args[0]);

    Handle<ObjectTemplate> global = ObjectTemplate::New();

    global->Set(String::New("api"),api);


    Persistent<Context> context = Context::New(NULL, global);

    Context::Scope context_scope(context);

    Handle<String> source = String::New(readFile(*ascii));

    Handle<Script> script = Script::Compile(source);

    if (*script == 0) {
        printf ("Error include %s : couldn't compile source ", (*ascii) );
        return Undefined();
    }

    Handle<Value> result = script->Run();

    context.Dispose();

    return result;
}





char* readFile(const char* file_name) {
    char* s;
    FILE * in = fopen(file_name, "r" );
    fseek( in, 0, SEEK_END);
    size_t file_size = ftell( in ) + 1;
    s = new char[file_size];
    fseek( in, 0, SEEK_SET);
    size_t n = fread(s, sizeof(char), file_size, in);
    s[file_size - 1] = '\0';
    n++;
    //printf ( " >>> %s <<< \n", s);
    return s;
}

void runScript (const char* filename) {
    HandleScope handle_scope;
    Handle<ObjectTemplate> global = ObjectTemplate::New();

    api = ObjectTemplate::New();
    api->Set(String::New("print"), FunctionTemplate::New(jsPrint));
    api->Set(String::New("init"), FunctionTemplate::New(jsInit));
    api->Set(String::New("exit"), FunctionTemplate::New(jsExit));
    api->Set(String::New("run"), FunctionTemplate::New(jsRun));
    api->Set(String::New("getRenderer"), FunctionTemplate::New(jsGetRenderer));
    api->Set(String::New("getInputDevice"), FunctionTemplate::New(jsGetInputDevice));
    api->Set(String::New("newScene"), FunctionTemplate::New(jsNewScene));
    api->Set(String::New("newSprite"), FunctionTemplate::New(jsNewSprite));
    api->Set(String::New("newImage"), FunctionTemplate::New(jsNewImage));


    global->Set(String::New("api"),api);
    global->Set(String::New("include"),FunctionTemplate::New(jsInclude));

    Persistent<Context> context = Context::New(NULL, global);

    Context::Scope context_scope(context);

    Handle<String> source = String::New(readFile(filename));

    Handle<Script> script = Script::Compile(source);

    Handle<Value> result = script->Run();

    context.Dispose();

    String::AsciiValue ascii(result);
    printf("\n\nScript result: %s\n", *ascii);
}
