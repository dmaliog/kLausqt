// Создание элемента загрузки
var loader = document.createElement('div');
loader.className = 'loader';
document.body.appendChild(loader);

// Функция для добавления глобальных стилей и удаления загрузчика
function addGlobalStyleWithLoader(css) {
    var head, style;
    head = document.getElementsByTagName('head')[0];
    if (!head) { return; }
    style = document.createElement('style');
    style.type = 'text/css';
    style.innerHTML = css;

    head.appendChild(style);
}

// Стили для загрузчика
var loaderStyle = `
    .loader {
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        background-color: rgba(0, 0, 0, 0.5);
        display: flex;
        justify-content: center;
        align-items: center;
        z-index: 9999;
    }

    .loader:before {
        content: "";
        width: 50px;
        height: 50px;
        border-radius: 50%;
        border: 6px solid #fff;
        border-top-color: transparent;
        animation: spin 1s linear infinite;
    }

    @keyframes spin {
        0% { transform: rotate(0deg); }
        100% { transform: rotate(360deg); }
    }
`;

// Добавление стилей загрузчика
addGlobalStyleWithLoader(loaderStyle);

// Удаление загрузчика после полной загрузки страницы
window.addEventListener('load', function() {
    document.body.removeChild(loader);

    // Применение глобальных стилей после удаления загрузчика
    addGlobalStyleWithLoader('#archnavbar,.footer{display:none;}');
});
