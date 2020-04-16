#include "StdFuture.h"
#include "Stream.h"

namespace Future {

//------------------------------------------------------------------------------
/**
*/
Stream::Stream() :
    accessMode_(AccessMode::ReadAccess),
    accessPattern_(AccessPattern::Sequential),
    isOpen_(false),
    isMapped_(false) {
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Stream::~Stream() {
    f_assert(!this->IsOpen());
    f_assert(!this->IsMapped());
}

//------------------------------------------------------------------------------
/**
This method must return true if the derived stream class supports reading.
*/
bool
Stream::CanRead() const {
    return false;
}

//------------------------------------------------------------------------------
/**
This method must return true if the derived stream class supports writing.
*/
bool
Stream::CanWrite() const {
    return false;
}

//------------------------------------------------------------------------------
/**
This method must return true if the derived stream supports seeking.
*/
bool
Stream::CanSeek() const {
    return false;
}

//------------------------------------------------------------------------------
/**
This method must return true if the stream supports direct memory
access through the Map()/Unmap() methods.
*/
bool
Stream::CanBeMapped() const {
    return false;
}

//------------------------------------------------------------------------------
/**
This sets a new size for the stream. Not all streams support this method.
If the new size if smaller then the existing size, the contents will
be clipped.
*/
void
Stream::SetSize(Size /*s*/) {
    f_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
This method returns the size of the stream in bytes.
*/
Stream::Size
Stream::GetSize() const {
    return 0;
}

//------------------------------------------------------------------------------
/**
This method returns the current position of the read/write cursor.
*/
Stream::Position
Stream::GetPosition() const {
    return 0;
}

Stream::Size
Stream::GetAvailable() const {
    return 0;
}

//------------------------------------------------------------------------------
/**
This method sets the intended access mode of the stream. The actual
behaviour depends on the implementation of the derived class. The
default is ReadWrite.
*/
void
Stream::SetAccessMode(AccessMode::Code m) {
    f_assert(!this->IsOpen());
    this->accessMode_ = m;
}

//------------------------------------------------------------------------------
/**
Get the access mode of the stream.
*/
Stream::AccessMode::Code
Stream::GetAccessMode() const {
    return this->accessMode_;
}

//------------------------------------------------------------------------------
/**
Set the prefered access pattern of the stream. This can be Random or
Sequential. This is an optional flag to improve performance with
some stream implementations. The default is sequential. The pattern
cannot be changed while the stream is open.
*/
void
Stream::SetAccessPattern(AccessPattern::Code p) {
    f_assert(!this->IsOpen());
    this->accessPattern_ = p;
}

//------------------------------------------------------------------------------
/**
Get the currently set prefered access pattern of the stream.
*/
Stream::AccessPattern::Code
Stream::GetAccessPattern() const {
    return this->accessPattern_;
}

//------------------------------------------------------------------------------
/**
Open the stream. Only one thread may open a stream at any time.
Returns true if succeeded.
*/
bool
Stream::Open() {
    f_assert(!this->IsOpen());
    this->isOpen_ = true;
    return true;
}

//------------------------------------------------------------------------------
/**
Closes the stream.
*/
void
Stream::Close() {
    f_assert(this->IsOpen());
    this->isOpen_ = false;
}

//------------------------------------------------------------------------------
/**
Return true if the stream is currently open.
*/
bool
Stream::IsOpen() const {
    return this->isOpen_;
}

//------------------------------------------------------------------------------
/**
Write raw data to the stream. For more convenient writing, attach the
stream to an IO::StreamWriter object. This method is only valid if the
stream class returns true in CanWrite().
*/
Stream::Size
Stream::Write(const void* /*ptr*/, Size /*numBytes*/) {
    f_assert(this->IsOpen());
    f_assert(!this->isMapped_);
    return 0;
}

//------------------------------------------------------------------------------
/**
Read raw data from the stream. For more convenient reading, attach
the stream to an IO::StreamReader object. The method returns the number
of bytes actually read. This method is only valid if the stream
class returns true in CanRead(). Returns the number of bytes actually
read from the stream, this may be less then numBytes, or 0 if end-of-stream
is reached.
*/
Stream::Size
Stream::Read(void* /*ptr*/, Size /*numBytes*/) {
    f_assert(this->IsOpen());
    f_assert(!this->isMapped_);
    return 0;
}

//------------------------------------------------------------------------------
/**
Move the read/write cursor to a new position, returns the new position
in the stream. This method is only supported if the stream class
returns true in CanSeek().
*/
bool
Stream::Seek(Offset /*offset*/, SeekOrigin::Code /*origin*/) {
    f_assert(this->IsOpen());
    f_assert(!this->isMapped_);
    return false;
}

bool 
Stream::Advance(Offset offset) {
    return this->Seek(offset, SeekOrigin::Current);
}

//------------------------------------------------------------------------------
/**
Flush any unsaved data. Note that unsaved data will also be flushed
automatically when the stream is closed.
*/
void
Stream::Flush() {
    f_assert(this->IsOpen());
    f_assert(!this->isMapped_);
}

//------------------------------------------------------------------------------
/**
Return true if the read/write cursor is at the end of the stream.
*/
bool
Stream::Eof() const {
    f_assert(this->IsOpen());
    f_assert(!this->isMapped_);
    return true;
}

//------------------------------------------------------------------------------
/**
If the stream provides memory mapping, this method will return a pointer
to the beginning of the stream data in memory. The application is
free to read and write to the stream through direct memory access. Special
care must be taken to not read or write past the end of the mapped data
(indicated by GetSize()). The normal Read()/Write() method are not
valid while the stream is mapped, also the read/write cursor position
will not be updated.
*/
void*
Stream::Map() {
    f_assert(this->IsOpen());
    f_assert(this->CanBeMapped());
    f_assert(!this->isMapped_);
    this->isMapped_ = true;
    return 0;
}

//------------------------------------------------------------------------------
/**
This will unmap a memory-mapped stream.
*/
void
Stream::Unmap() {
    f_assert(this->IsOpen());
    f_assert(this->CanBeMapped());
    f_assert(this->isMapped_);
    this->isMapped_ = false;
}

//------------------------------------------------------------------------------
/**
Returns true if the stream is currently mapped.
*/
bool
Stream::IsMapped() const {
    return this->isMapped_;
}

void* 
Stream::MappedConntent() {
    return nullptr;
}

} // namespace Future