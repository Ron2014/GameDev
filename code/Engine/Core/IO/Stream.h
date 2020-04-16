#pragma once
#include "Core/Class.h"
#include "Core/RefCounted.h"
#include <cstdint>

//------------------------------------------------------------------------------
namespace Future {
class Stream : public RefCounted {
    FutureClassDecl(Stream);
    FutureBaseTypeDecl(Stream);
public:
    /// typedefs
    typedef int32_t Position;       // DON'T DEFINE AS UNSIGNED!
    typedef int32_t Offset;         // DON'T DEFINE AS UNSIGNED!
    typedef int32_t Size;           // DON'T DEFINE AS UNSIGNED!

                                /// access modes
    struct AccessMode {
        enum Code {
            ReadAccess,
            WriteAccess,
            AppendAccess,
            ReadWriteAccess,
        };
    };

    /// access prefered pattern
    struct AccessPattern {
        enum Code {
            Random,
            Sequential,
        };
    };

    /// seek origins
    struct SeekOrigin {
        enum Code {
            Begin,
            Current,
            End,
        };
    };
    /// constructor
    Stream();
    /// destructor
    virtual ~Stream();

    /// return true if the stream supports reading
    virtual bool CanRead() const;
    /// return true if the stream supports writing
    virtual bool CanWrite() const;
    /// return true if the stream supports seeking
    virtual bool CanSeek() const;
    /// return true if the stream provides direct memory access
    virtual bool CanBeMapped() const;
    /// set a new size for the stream
    virtual void SetSize(Size s);
    /// get the size of the stream in bytes
    virtual Size GetSize() const;
    /// get the current position of the read/write cursor
    virtual Position GetPosition() const;
    /// get the available of the stream in bytes
    virtual Size GetAvailable() const;
    /// set the access mode of the stream (default is ReadAccess)
    void SetAccessMode(AccessMode::Code m);
    /// get the access mode of the stream
    AccessMode::Code GetAccessMode() const;
    /// set the prefered access pattern (default is Sequential)
    void SetAccessPattern(AccessPattern::Code p);
    /// get the prefered access pattern
    AccessPattern::Code GetAccessPattern() const;
    /// open the stream
    virtual bool Open();
    /// close the stream
    virtual void Close();
    /// return true if currently open
    bool IsOpen() const;
    /// directly write to the stream
    virtual Size Write(const void* ptr, Size numBytes);
    /// directly read from the stream
    virtual Size Read(void* ptr, Size numBytes);
    /// seek in stream
    virtual bool Seek(Offset offset, SeekOrigin::Code origin);

    virtual bool Advance(Offset offset);
    /// flush unsaved data
    virtual void Flush();
    /// return true if end-of-stream reached
    virtual bool Eof() const;
    /// map stream to memory
    virtual void* Map();
    /// unmap stream
    virtual void Unmap();
    /// return true if stream is currently mapped to memory
    bool IsMapped() const;

    virtual void* MappedConntent();

    template<typename T = int8_t>
    T* MappedMemoryPtr(Position position = 0) {
        if (!IsMapped()) {
            return nullptr;
        }
        if (position < this->GetSize()) {
            auto ptr = (int8_t*)this->MappedConntent();
            if (ptr) {
                return ptr + position;
            }
        }
        return nullptr;
    }

protected:
    AccessMode::Code accessMode_;
    AccessPattern::Code accessPattern_;
    bool isOpen_;
    bool isMapped_;
};

} // namespace Future
