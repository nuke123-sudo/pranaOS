test("length is 2", () => {
    expect(Array.prototype.splice).toHaveLength(2);
});

test("basic functionality", () => {
    var array = ["hello", "friends", "pranaOS", 1, 2];
    var removed = array.splice(3);
    expect(array).toEqual(["hello", "friends", "pranaOS"]);
    expect(removed).toEqual([1, 2]);

    array = ["hello", "friends", "pranaOS", 1, 2];
    removed = array.splice(-2);
    expect(array).toEqual(["hello", "friends", "pranaOS"]);
    expect(removed).toEqual([1, 2]);

    array = ["hello", "friends", "pranaOS", 1, 2];
    removed = array.splice(-2, 1);
    expect(array).toEqual(["hello", "friends", "pranaOS", 2]);
    expect(removed).toEqual([1]);

    array = ["pranaOS"];
    removed = array.splice(0, 0, "hello", "friends");
    expect(array).toEqual(["hello", "friends", "pranaOS"]);
    expect(removed).toEqual([]);

    array = ["goodbye", "friends", "pranaOS"];
    removed = array.splice(0, 1, "hello");
    expect(array).toEqual(["hello", "friends", "pranaOS"]);
    expect(removed).toEqual(["goodbye"]);

    array = ["foo", "bar", "baz"];
    removed = array.splice();
    expect(array).toEqual(["foo", "bar", "baz"]);
    expect(removed).toEqual([]);

    removed = array.splice(0, 123);
    expect(array).toEqual([]);
    expect(removed).toEqual(["foo", "bar", "baz"]);

    array = ["foo", "bar", "baz"];
    removed = array.splice(123, 123);
    expect(array).toEqual(["foo", "bar", "baz"]);
    expect(removed).toEqual([]);

    array = ["foo", "bar", "baz"];
    removed = array.splice(-123, 123);
    expect(array).toEqual([]);
    expect(removed).toEqual(["foo", "bar", "baz"]);
});

test.skip("Invalid lengths", () => {
    var length = Math.pow(2, 32);

    var obj = {
        length: length,
    };

    expect(() => {
        Array.prototype.splice.call(obj, 0);
    }).toThrowWithMessage(RangeError, "Invalid array length");
});