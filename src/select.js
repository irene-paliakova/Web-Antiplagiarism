const header = document.querySelector(".select__header");
const body = document.querySelector(".select__body");
const btn = document.querySelector("#send");
const input = document.querySelector("#value");

header.addEventListener('click', () => {
    body.classList.toggle('active');
    header.classList.toggle('active');
}) ;

input.addEventListener('keyup', () => {
   let inputArr = input.value.trim().split(' ');
    if (inputArr.length <=2) {
        btn.classList.add('disabled');
        btn.disabled = true;
    } else {
        btn.classList.remove('disabled');
        btn.disabled = false;
    }
});



