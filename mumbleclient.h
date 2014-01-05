// Written by Adrian Musceac YO8RZZ at gmail dot com, started October 2013.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef MUMBLECLIENT_H
#define MUMBLECLIENT_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QtEndian>
#include <string>
#include <sys/time.h>
#include "ext/Mumble.pb.h"
#include "ext/CryptState.h"
#include "ext/PacketDataStream.h"
#include "ext/utils.h"
#include "sslclient.h"
#include "audiointerface.h"
#include "audioencoder.h"
#include "opus/opus.h"
#include "config_defines.h"


class MumbleClient : public QObject
{
    Q_OBJECT
public:
    explicit MumbleClient(QObject *parent = 0);
    ~MumbleClient();
    void connectToServer(QString address, unsigned port);
    void disconnectFromServer();

    QString getChannelName();
    int getChannelId();
    QString createChannel();
    void joinChannel(int id);
signals:
    void channelName(QString name);
    void pcmAudio(short *pcm, short size);
    
public slots:
    void sendVersion();
    void authenticate();
    void pingServer();
    void processProtoMessage(QByteArray data);
    void processUDPData(QByteArray data);
    void sendUDPPing();
    void processAudio(short *audiobuffer, short audiobuffersize);

private:
    void sendUDPMessage(quint8 *message, int size);
    void sendMessage(quint8 *message, quint16 type, int size);
    void setupEncryption(quint8 *message, quint64 size);

    void processServerSync(quint8 *message, quint64 size);
    void processChannelState(quint8 *message, quint64 size);
    void processUserState(quint8 *message, quint64 size);
    void createVoicePacket(unsigned char *encoded_audio, int packet_size);
    void processIncomingAudioPacket(quint8 *data, quint64 size);
    void decodeAudio(unsigned char *audiobuffer, short audiobuffersize);

    SSLClient *_telnet;
    CryptState *_crypt_state;
    std::string _key;
    std::string _client_nonce;
    std::string _server_nonce;
    QString _temp_channel_name;
    bool _encryption_set;
    bool _synchronized;
    bool _authenticated;
    int _session_id;
    int _max_bandwidth;
    int _channel_id;
    OpusEncoder *_opus_encoder;
    OpusDecoder *_opus_decoder;
    AudioEncoder *_codec;
    quint64 _sequence_number;

    
};


#endif // MUMBLECLIENT_H