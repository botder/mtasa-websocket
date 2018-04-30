/*****************************************************************************
 *
 *  PROJECT:     Websocket module
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        CLuaArgument.h
 *  COPYRIGHT:   Copyright (c) 2003-2018 MTA
 *               Grand Theft Auto (c) 2002-2003 Rockstar North
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/
#pragma once

#include "CLuaArguments.h"
#include <string>

struct lua_State;

class CLuaArgument
{
public:
    virtual ~CLuaArgument() = default;
    virtual unsigned int Push(lua_State *);
    virtual int GetType();
    bool HasValue();
};

template <typename T>
class CLuaType : public CLuaArgument
{
public:
    CLuaType(T value) : m_Value(value) {}
    T& GetValue() { return m_Value; }

protected:
    T m_Value;
};

class CLuaNil : public CLuaArgument
{
public:
    unsigned int Push(lua_State *L) override;
    int GetType() override;
};

class CLuaNumber : public CLuaType<double>
{
public:
    using CLuaType::CLuaType;
    unsigned int Push(lua_State *L) override;
    int GetType() override;
};

class CLuaBoolean : public CLuaType<bool>
{
public:
    using CLuaType::CLuaType;
    unsigned int Push(lua_State *L) override;
    int GetType() override;
};

class CLuaString : public CLuaType<std::string>
{
public:
    using CLuaType::CLuaType;
    unsigned int Push(lua_State *L) override;
    int GetType() override;
};

class CLuaUserData : public CLuaType<void *>
{
public:
    using CLuaType::CLuaType;
    unsigned int Push(lua_State *L) override;
    int GetType() override;
};

class CLuaReference : public CLuaType<int>
{
public:
    CLuaReference();
    CLuaReference(lua_State *L, int i);
    ~CLuaReference() override;
    CLuaReference(const CLuaReference &other) = delete;
    CLuaReference& operator=(const CLuaReference &other) = delete;
    CLuaReference(CLuaReference &&other);
    CLuaReference& operator=(CLuaReference &&other);
    int GetType() override;
    int GetReferenceType();

protected:
    int m_Type;
    lua_State *m_State;
};

class CLuaTable : public CLuaReference
{
public:
    using CLuaReference::CLuaReference;
    unsigned int Push(lua_State *L) override;
    int GetType() override;
};

class CLuaFunction : public CLuaReference
{
public:
    using CLuaReference::CLuaReference;
    unsigned int Push(lua_State *L) override;
    int GetType() override;

public:
    CLuaArguments Call(const CLuaArguments &arguments);
    CLuaArguments Call(lua_State *L, const CLuaArguments &arguments);
    CLuaArguments operator()(const CLuaArguments &arguments);
    CLuaArguments operator()(lua_State *L, const CLuaArguments &arguments);
};

class CLuaThread : public CLuaReference
{
public:
    using CLuaReference::CLuaReference;
    unsigned int Push(lua_State *L) override;
    int GetType() override;
};
