//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
//  RAII sentries for opengl
#pragma once

struct GLEnableSentry
{
	GLEnableSentry(GLenum op) : operation(op)
	{
		glEnable(operation);
	}

	~GLEnableSentry()
	{
		glDisable(operation);
	}

	const GLenum operation;
};

struct GLDisableSentry
{
	GLDisableSentry(GLenum op) : operation(op)
	{
		glDisable(operation);
	}

	~GLDisableSentry()
	{
		glEnable(operation);
	}

	const GLenum operation;
};

struct GLEnableAttribSentry
{
    GLEnableAttribSentry(int i) : index(i)
    {
        glEnableVertexAttribArray(index);
    }

    ~GLEnableAttribSentry()
    {
        glDisableVertexAttribArray(index);
    }

    const int index;
};

#define SENTRY(sentry, op) sentry PP_CAT(sentry, __LINE__)(op)