define(function() {
    let friends = [];
    let pictures = {};
    let friend_to_show = null;

    function add_friend (_name, _pic) {
        friends.push({
            name: _name,
            pic: _pic
        });

        pictures[_name] = _pic;
    }

    add_friend("João M.",   "./img/avatar1.jpg");
    add_friend("Maria J.",  "./img/avatar2.jpg");
    add_friend("Manuel C.", "./img/avatar3.jpg");
    add_friend("Tiago K.",  "./img/avatar4.jpg");
    add_friend("José M.",   "./img/avatar5.jpg");
    add_friend("Joana R.",  "./img/avatar6.jpg");
    add_friend("Leonor T.", "./img/avatar7.jpg");

    return {
        friends: friends,
        friend_to_show: friend_to_show,
        pictures: pictures
    }
});