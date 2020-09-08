# fty-cmake-rest
Helper for creating tntnet rest servlet with cmake

# How to use
See example folder.

# Other way how to create tntnet method

This way could be more simpliest and natural than ecpp generate.

Example (with use of fty::pack):


Header:
```
#pragma once
#include <fty/rest-support.h>

class MyMethod : public rest::Runner
{
public:
    static constexpr const char* NAME = "my-method";

public:
    using rest::Runner::Runner;
    unsigned run() override;
};
```


Implementation:
```
#include "mymethod.h"
#include <pack/pack.h>

// Input meta class definition
class Params : public pack::Node
{
public:
    pack::String name  = FIELD("name");
    pack::Int    value = FIELD("value");
public:
    using pack::Node::Node;
    META(Params, name, value);
};

// Output meta class definition
class Output: public pack::Node
{
public:
    pack::String res      = FIELD("res");
    pack::Int    someCode = FIELD("code");
public:
    using pack::Node::Node;
    META(Output, res, someCode);
};

// Run our method
unsigned MyMethod::run()
{
    Params params;

    // This is a post method with json input parameters (defininition in Params class)
    if (auto res = pack::json::deserialize(m_request.getBody(), params); !res) {
        // Something going wrong, you could log res.error() or append this error to output.
        // In example just returns "bad-input" error.
        auto err = rest::error("bad-input", m_request.getBody());
        m_reply.out() << err.message << "\n\n";
        return err.code;
    }

    Output out;

    // Do something
    out.res      = "MyMethod result";
    out.someCode = 42;

    m_reply.out() << *pack::json::serialize(out) << "\n\n";
    return HTTP_OK;
}

// Register MyMethod as tntnet handler, could be in impl or in header. Just call it anywhere :)
registerHandler(MyMethod)
```

