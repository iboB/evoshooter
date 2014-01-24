//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
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

#define SENTRY(sentry, op) sentry PP_CAT(sentry, __LINE__)(op)