import React from 'react';

const Message = (props) => {
    return (
        <div className="d">
            {props.text}
            {props.m}
        </div>
    );
};

export default Message;