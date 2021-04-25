const header = document.querySelector(".select__header");
const body = document.querySelector(".select__body");

header.addEventListener('click', () => {
    body.classList.toggle('active');
    header.classList.toggle('active');
}) ;

