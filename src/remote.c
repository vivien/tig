/* Copyright (c) 2014 Vivien Didelot <vivien.didelot@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "pager.h"

static bool
remote_open(struct view *view, enum open_flags flags)
{
	static const char *remote_argv[] = { "git", "remote", NULL };

	return begin_update(view, NULL, remote_argv, flags | OPEN_RELOAD);
}

static void
remote_select(struct view *view, struct line *line)
{
	string_format(view->env->remote, "%s", line->data);
	string_copy(view->ref, view->env->remote);
}

static enum request
remote_request(struct view *view, enum request request, struct line *line)
{
	struct remote_state *state = view->private;
	static const char *remote_argv[] = {
		"git", "remote", "show", "%(remote)", NULL
	};

	switch (request) {
	case REQ_REFRESH:
		refresh_view(view);
		return REQ_NONE;

	case REQ_ENTER:
		/* XXX How to reenter the same view with the new remote_argv? */
		open_argv(view, VIEW(REQ_VIEW_REMOTE), remote_argv, NULL, OPEN_SPLIT);
		return REQ_NONE;

	default:
		return request;
	}
}

struct view_ops remote_ops = {
	"remote",
	{ "remote" },
	argv_env.remote,
	VIEW_REFRESH,
	0,
	remote_open,
	pager_read,
	pager_draw,
	remote_request,
	pager_grep,
	remote_select,
};

/* vim: set ts=8 sw=8 noexpandtab: */
