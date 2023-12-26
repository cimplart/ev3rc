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

#include "rccommandpublisher.h"
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include "ev3dataPubSubTypes.h"
#include <QDebug>

using namespace eprosima::fastdds::dds;

RcCommandPublisher::RcCommandPublisher() : myMoveCommandType(new Ev3MoveCommandPubSubType())
{
    DomainParticipantQos pqos = PARTICIPANT_QOS_DEFAULT;
    //pqos.name("Participant_pub");
    myParticipant = DomainParticipantFactory::get_instance()->create_participant(myDomain, pqos);
    if (myParticipant == nullptr) {
        throw std::runtime_error("Failed creating domain participant");
    }

    myMoveCommandType.register_type(myParticipant);

    myPublisher = myParticipant->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
    if (myPublisher == nullptr) {
        DomainParticipantFactory::get_instance()->delete_participant(myParticipant);
        throw std::runtime_error("Failed creating RC command publisher");
    }

    myRcCommandTopic = myParticipant->create_topic("ev3_move_command_topic", myMoveCommandType.get_type_name(),
                                                     TOPIC_QOS_DEFAULT);
    if (myRcCommandTopic == nullptr)  {
        myParticipant->delete_publisher(myPublisher);
        DomainParticipantFactory::get_instance()->delete_participant(myParticipant);
        throw std::runtime_error("Failed creating EV3 move command topic");
    }

    DataWriterQos wqos = myPublisher->get_default_datawriter_qos();
    wqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
//    wqos.liveliness().lease_duration = 5;
//    wqos.liveliness().announcement_period = 1;
//    wqos.liveliness().kind = AUTOMATIC_LIVELINESS_QOS;

    myWriter = myPublisher->create_datawriter(myRcCommandTopic, wqos, &myListener);
    if (myWriter == nullptr)  {
        myParticipant->delete_topic(myRcCommandTopic);
        myParticipant->delete_publisher(myPublisher);
        DomainParticipantFactory::get_instance()->delete_participant(myParticipant);
        throw std::runtime_error("Failed creating RC command writer");
    }
}

RcCommandPublisher::~RcCommandPublisher()
{
    assert(myPublisher != nullptr);
    myPublisher->delete_datawriter(myWriter);
    assert(myParticipant != nullptr);
    myParticipant->delete_publisher(myPublisher);
    myParticipant->delete_topic(myRcCommandTopic);
    DomainParticipantFactory::get_instance()->delete_participant(myParticipant);
}

void RcCommandPublisher::PubListener::on_publication_matched(
         eprosima::fastdds::dds::DataWriter* writer,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info)
{
    qDebug("Publication matched");
}

void RcCommandPublisher::PubListener::on_offered_incompatible_qos(eprosima::fastdds::dds::DataWriter *writer,
                                  const eprosima::fastdds::dds::OfferedIncompatibleQosStatus &status)
{
    qDebug("on_offered_incompatible_qos, last_policy_id=%d", status.last_policy_id);
}


void RcCommandPublisher::send_command(Ev3MoveType move, bool make)
{
    void* sample = myMoveCommandType->createData();
    Ev3MoveCommand *asCmd = static_cast<Ev3MoveCommand*>(sample);

    asCmd->move(move);
    asCmd->make(make);
    if ( !myWriter->write(sample)) {
        qDebug("Writing sample failed");
    }
}
