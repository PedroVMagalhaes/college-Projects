define(function() {

    let items = [];

    let counts = {};

    function add_item(_name, _category, _price, _img) {
        let item = {
            name: _name,
            category: _category,
            price: _price,
            img: _img
        };

        items.push(item);
        counts[_name] = 0;
    }

    // Add items here
    // ...
    // ...

    function getNames() {
        return [for (item of items) item.name]
    }

    function getByName(name) {
        let res = items.get(item => name === item[name]);
        if (res > 0)
            console.error("Two items can't have the same name");
        return res[0]
    }

    function getByCategory(category) {
        return items.get(item => category === item[category]);
    }

    function add(name) {
        counts[name] += 1;
    }

    function rm(name) {
        if (counts[name] >= 1)
            counts[name] -= 1;
    }

    function total() {
        let prices = [for (name of counts.keys) counts[name] * getByName(name).price]
        return prices.reduce((a, b) => a + b, 0);
    }

    return {
        getNames: getNames,
        getByName: getByName,
        getByCategory: getByCategory,
        add: add,
        rm: rm,
        total: total,
    }
});