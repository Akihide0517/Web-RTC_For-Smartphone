const { nowInSec, SkyWayAuthToken, SkyWayContext, SkyWayRoom, SkyWayStreamFactory, uuidV4 } = skyway_room;
const token = new SkyWayAuthToken({
  jti: uuidV4(),
  iat: nowInSec(),
  exp: nowInSec() + 60 * 60 * 24,
  scope: {
    app: {
      id: '683f22f4-cfdc-4e07-8b43-e9c82516bdbd',
      turn: true,
      actions: ['read'],
      channels: [
        {
          id: '*',
          name: '*',
          actions: ['write'],
          members: [
            {
              id: '*',
              name: '*',
              actions: ['write'],
              publication: {
                actions: ['write'],
              },
              subscription: {
                actions: ['write'],
              },
            },
          ],
          sfuBots: [
            {
              actions: ['write'],
              forwardings: [
                {
                  actions: ['write'],
                },
              ],
            },
          ],
        },
      ],
    },
  },
}).encode('Ub/E/BkYcAntyOlfwBf5CJiibNJ/V8o9CmD2ogHbh80=');

(async () => {
  const localVideo = document.getElementById('local-video');
  const buttonArea = document.getElementById('button-area');
  const remoteMediaArea = document.getElementById('remote-media-area');
  const roomNameInput = document.getElementById('room-name');
  const chatArea = document.getElementById('chat-area');
  const chatInput = document.getElementById('chat-input');
  const sendChatButton = document.getElementById('send-chat');

  const myId = document.getElementById('my-id');
  const joinButton = document.getElementById('join');

  const { audio, video } = await SkyWayStreamFactory.createMicrophoneAudioAndCameraStream();
  video.attach(localVideo);
  await localVideo.play();

  let myMember;
  let chatStream;

  joinButton.onclick = async () => {
    if (roomNameInput.value === '') return;

    const context = await SkyWayContext.Create(token);
    const room = await SkyWayRoom.FindOrCreate(context, {
      type: 'p2p',
      name: roomNameInput.value,
    });
    const me = await room.join();

    myId.textContent = me.id;
    myMember = me;

    await me.publish(audio);
    await me.publish(video);

    // テキストチャット用のデータストリームを作成して公開
    chatStream = await SkyWayStreamFactory.createDataStream({ label: 'chat' });
    await me.publish(chatStream);

    sendChatButton.onclick = () => {
      const message = chatInput.value;
      if (message === '') return;

      // メッセージを送信し、ローカルにも表示
      chatStream.write(message);
      const myMessage = document.createElement('div');
      myMessage.textContent = `自分: ${message}`;
      chatArea.appendChild(myMessage);
      chatInput.value = '';
    };

    const subscribeAndAttach = async (publication) => {
      if (publication.publisher.id === me.id) return;

      if (publication.contentType === 'data') {
        // データストリームの購読とメッセージ受信
        const { stream } = await me.subscribe(publication.id);
        stream.onData.add((data) => {
          const message = document.createElement('div');
          message.textContent = `${publication.publisher.id}: ${data}`;
          chatArea.appendChild(message);
        });
        return;
      }

      const subscribeButton = document.createElement('button');
      subscribeButton.textContent = `${publication.publisher.id}: ${publication.contentType}`;
      buttonArea.appendChild(subscribeButton);

      subscribeButton.onclick = async () => {
        const { stream } = await me.subscribe(publication.id);

        let newMedia;
        switch (stream.track.kind) {
          case 'video':
            newMedia = document.createElement('video');
            newMedia.playsInline = true;
            newMedia.autoplay = true;
            break;
          case 'audio':
            newMedia = document.createElement('audio');
            newMedia.controls = true;
            newMedia.autoplay = true;
            break;
          default:
            return;
        }
        stream.attach(newMedia);
        remoteMediaArea.appendChild(newMedia);
      };
    };

    room.publications.forEach(subscribeAndAttach);
    room.onStreamPublished.add((e) => subscribeAndAttach(e.publication));
  };
})();
