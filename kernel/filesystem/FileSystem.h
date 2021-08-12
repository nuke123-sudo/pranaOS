/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RefCounted.h>
#include <base/RefPtr.h>
#include <base/StringView.h>
#include <kernel/filesystem/InodeIdentifier.h>
#include <kernel/Forward.h>
#include <kernel/KResult.h>
#include <kernel/locking/Mutex.h>
#include <kernel/UnixTypes.h>
#include <kernel/UserOrKernelBuffer.h>

namespace Kernel {

static constexpr u32 mepoch = 476763780;

class FileSystem : public RefCounted<FileSystem> {
    friend class Inode;

public:
    virtual ~FileSystem();

    unsigned fsid() const { return m_fsid; }
    static FileSystem* from_fsid(u32);
    static void sync();
    static void lock_all();

    virtual bool initialize() = 0;
    virtual StringView class_name() const = 0;
    virtual Inode& root_inode() = 0;
    virtual bool supports_watchers() const { return false; }

    bool is_readonly() const { return m_readonly; }

    virtual unsigned total_block_count() const { return 0; }
    virtual unsigned free_block_count() const { return 0; }
    virtual unsigned total_inode_count() const { return 0; }
    virtual unsigned free_inode_count() const { return 0; }

    virtual KResult prepare_to_unmount() { return KSuccess; }

    struct DirectoryEntryView {
        DirectoryEntryView(const StringView& name, InodeIdentifier, u8 file_type);

        StringView name;
        InodeIdentifier inode;
        u8 file_type { 0 };
    };

    virtual void flush_writes() { }

    u64 block_size() const { return m_block_size; }
    size_t fragment_size() const { return m_fragment_size; }

    virtual bool is_file_backed() const { return false; }

    virtual u8 internal_file_type_to_directory_entry_type(const DirectoryEntryView& entry) const { return entry.file_type; }

protected:
    FileSystem();

    void set_block_size(u64 size) { m_block_size = size; }
    void set_fragment_size(size_t size) { m_fragment_size = size; }

    mutable Mutex m_lock { "FS" };

private:
    unsigned m_fsid { 0 };
    u64 m_block_size { 0 };
    size_t m_fragment_size { 0 };
    bool m_readonly { false };
};

inline FileSystem* InodeIdentifier::fs()
{
    return FileSystem::from_fsid(m_fsid);
}

inline const FileSystem* InodeIdentifier::fs() const
{
    return FileSystem::from_fsid(m_fsid);
}

}

namespace Base {

template<>
struct Traits<Kernel::InodeIdentifier> : public GenericTraits<Kernel::InodeIdentifier> {
    static unsigned hash(const Kernel::InodeIdentifier& inode) { return pair_int_hash(inode.fsid(), inode.index().value()); }
};

}