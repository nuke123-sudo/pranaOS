/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/StringView.h>
#include <kernel/filesystem/VirtualFileSystem.h>
#include <kernel/Process.h>

namespace Kernel {

KResultOr<FlatPtr> Process::sys$readlink(Userspace<const Syscall::SC_readlink_params*> user_params)
{
    VERIFY_PROCESS_BIG_LOCK_ACQUIRED(this)
    REQUIRE_PROMISE(rpath);

    Syscall::SC_readlink_params params;
    if (!copy_from_user(&params, user_params))
        return EFAULT;

    auto path = get_syscall_path_argument(params.path);
    if (path.is_error())
        return path.error();

    auto result = VirtualFileSystem::the().open(path.value()->view(), O_RDONLY | O_NOFOLLOW_NOERROR, 0, current_directory());
    if (result.is_error())
        return result.error();
    auto description = result.value();

    if (!description->metadata().is_symlink())
        return EINVAL;

    auto contents = description->read_entire_file();
    if (contents.is_error())
        return contents.error();

    auto& link_target = *contents.value();
    auto size_to_copy = min(link_target.size(), params.buffer.size);
    if (!copy_to_user(params.buffer.data, link_target.data(), size_to_copy))
        return EFAULT;

    return link_target.size();
}

}