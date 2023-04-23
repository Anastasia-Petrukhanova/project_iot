import React, {useEffect, useState} from 'react';
import './Styles/App.css';
import Message from "./Components/message";

function App() {
    const [m, setM] = useState([])
    const [cont, setCont] = useState('')
    const [data, setData] = useState({
        "value": 0,
        "message": ''
    })
    const[c, setC] = useState('')

    const addTV = async (e) => {
        e.preventDefault()
        if (cont >= 0 && cont <= 30) {
            const d = {"value": cont}
            const response = await fetch('/tmp-val', {
                method: "POST",
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(d)
            })
            if (response.ok) {
                const res = await fetch('/tmp-val1')
                const data = await res.json()
                setData({
                    "value": data.value,
                    "message": data.message
                })
                console.log(data)
                const newM = {
                    id: Date.now(),
                    number: data.value,
                    text: data.message
                }
                setM([newM])
            }
        }
        else {
            const newM = {
                id: Date.now(),
                number: '',
                text: "Неверное значение"
            }
            setM([newM])
        }
    }
    const addLevel = async (e) => {
        e.preventDefault()
        const res = await fetch('/level')
        const data = await res.json()
        setData({
            "value": data.value,
            "message": data.message
        })
        const newM = {
            id: Date.now(),
            number: data.value,
            text: data.message
        }
        setM([newM])
    }
    const addTmp = async (e) => {
        e.preventDefault()
        const res = await fetch('/tmp')
        const data = await res.json()
        setData({
            "value": data.value,
            "message": data.message
        })
        const newM = {
            id: Date.now(),
            number: data.value,
            text: data.message
        }
        setM([newM])
    }

    const handleChange = event => {
        const result = event.target.value.replace(/\D/g, '');
        setCont(result);
      };

    useEffect(() => {
        fetch("/start").then((res) =>
            res.json().then((c) => {
                setC(c);
            })
        );
    }, []);

  return (
    <div className="App">
        <h1 className="h1">Система "Умный гараж"</h1>
        <div className="frame">
            <div className="d">
                <div className="d">
                    <button className="b" onClick={addLevel}> Узнать уровень снега </button>
                </div>
                <div className="d">
                    <button className="b" onClick={addTmp}> Узнать температуру в гараже </button>
                </div>
                <div className="d">
                    <form>
                        <label>Установить температуру</label>
                        <input
                            value={cont}
                            onChange={handleChange}
                            placeholder="от 0 до 30"
                            type="text"
                        />
                        <button className="b" onClick={addTV}> OK </button>
                    </form>
                </div>
            </div>
        </div>
        {m.map((m1) =>
            <Message m = {m1.number} text = {m1.text} key = {m1.id}/>
        )}
    </div>
  );
}

export default App;
