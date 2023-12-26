// MIT License

// Copyright (c) 2023 Artur Wisz

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef RCCOMMANDPUBLISHER_H
#define RCCOMMANDPUBLISHER_H

#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <memory>
#include "ev3data.h"

class RcCommandPublisher
{
public:
    RcCommandPublisher();
    virtual ~RcCommandPublisher();

    void send_command(Ev3MoveType move, bool make);

private:

    static constexpr eprosima::fastdds::dds::DomainId_t myDomain = 0;
    eprosima::fastdds::dds::DomainParticipant *myParticipant = nullptr;
    eprosima::fastdds::dds::Publisher *myPublisher = nullptr;
    eprosima::fastdds::dds::Topic *myRcCommandTopic = nullptr;
    eprosima::fastdds::dds::DataWriter *myWriter = nullptr;

    eprosima::fastdds::dds::TypeSupport myMoveCommandType;

    using DomainParticipant = eprosima::fastdds::dds::DomainParticipant;

    class PubListener : public eprosima::fastdds::dds::DataWriterListener
    {
    public:

        PubListener()
        //    : matched_(0)
        //    , firstConnected_(false)
        {
        }

        ~PubListener() override
        {
        }

        void on_publication_matched(eprosima::fastdds::dds::DataWriter* writer,
                 const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;

        void on_offered_incompatible_qos(eprosima::fastdds::dds::DataWriter *writer,
                                          const eprosima::fastdds::dds::OfferedIncompatibleQosStatus &status) override;

        //int matched_;

        //bool firstConnected_;
    } myListener;

};

#endif // RCCOMMANDPUBLISHER_H
